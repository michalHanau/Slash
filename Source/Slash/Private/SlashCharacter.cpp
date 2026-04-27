// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/SlashCharacter.h"
#include "Animation/AnimMontage.h"
#include "Animation/AnimInstance.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GroomComponent.h"
#include "Items/Weapon.h"
#include "Components/BoxComponent.h"


ASlashCharacter::ASlashCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->RotationRate = FRotator(0.f, 400.f, 0.f);

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f;

	ViewCamera = CreateDefaultSubobject<UCameraComponent>(FName("ViewCamera"));
	ViewCamera->SetupAttachment(CameraBoom);

	Hair = CreateDefaultSubobject<UGroomComponent>(FName("Hair"));
	Hair->SetupAttachment(GetMesh());
	Hair->AttachmentName = FString("head");

	Eyebrows = CreateDefaultSubobject<UGroomComponent>(FName("Eyebrows"));
	Eyebrows->SetupAttachment(GetMesh());
	Eyebrows->AttachmentName = FString("head");
}

void ASlashCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void ASlashCharacter::MoveForward(float Value)
{
	//אם הדמות במצב תקיפה לצאת
	if (ActionState == EActionState::EAS_Attacking) return;
	
	if (Controller && (Value != 0.f))
	{
		const FRotator ControlRotation = GetControlRotation();
		const FRotator YawRotation(0.f, ControlRotation.Yaw, 0.f);

		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void ASlashCharacter::MoveRight(float Value)
{
	//אם הדמות במצב תקיפה לצאת
	if (ActionState == EActionState::EAS_Attacking) return;
	
	if (Controller && (Value != 0.f))
	{
		const FRotator ControlRotation = GetControlRotation();
		const FRotator YawRotation(0.f, ControlRotation.Yaw, 0.f);

		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(Direction, Value);
	}
}

void ASlashCharacter::Turn(float Value)
{
	AddControllerYawInput(Value);
}

void ASlashCharacter::LookUp(float Value)
{
	AddControllerPitchInput(Value);
}

void ASlashCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASlashCharacter::EKeyPressed()
{
	if (OverlappingWeapon)
	{
		OverlappingWeapon->Equip(GetMesh(), FName("RightHandSocket"));
		CharacterState = ECharacterState::ECS_EquippedOneHandedWeapon;
	}
}

void ASlashCharacter::Attack()
{
	// בדיקה שהדמות מחזיקה חרב
	if (CharacterState == ECharacterState::ECS_Unequipped) return;

	// האם הדמות פנויה לתקוף
	if (ActionState == EActionState::EAS_Unoccupied)
	{
		PlayAttackMontage(); // מפעילים את האנימציה
		ActionState = EActionState::EAS_Attacking; // משנים את המצב ל"תוקף"
	}
}

void ASlashCharacter::PlayAttackMontage()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && AttackMontage)
	{
		// 1. הפעלת המונטאז'
		AnimInstance->Montage_Play(AttackMontage);

		// בחירת סוג התזוזה באופן רנדומלי
		const int32 Selection = FMath::RandRange(0, 1);
		FName SectionName = FName("");

		switch (Selection)
		{
		case 0:
			SectionName = FName("Attack1");
			break;
		case 1:
			SectionName = FName("Attack2");
			break;
		default:
			break;
		}

		// קציפה למצב שנבחר
		AnimInstance->Montage_JumpToSection(SectionName, AttackMontage);
	}
}

void ASlashCharacter::AttackEnd()
{
	ActionState = EActionState::EAS_Unoccupied;
}

void ASlashCharacter::SetWeaponCollisionEnabled(ECollisionEnabled::Type CollisionEnabled)
{
	// בדיקה: האם יש לנו נשק? והאם לנשק יש קופסת התנגשות?
	if (EquippedWeapon && EquippedWeapon->GetWeaponBox())
	{
		EquippedWeapon->GetWeaponBox()->SetCollisionEnabled(CollisionEnabled);
	}
}

void ASlashCharacter::EnableWeaponCollision()
{
	// הדפסה למסך לבדיקה
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(
			-1,              // ID (מינוס 1 אומר שההודעה תתווסף לרשימה ולא תחליף קיימת)
			5.f,             // כמה שניות ההודעה תישאר על המסך
			FColor::Blue,    // צבע הטקסט (בחרתי טורקיז שיהיה בולט)
			TEXT("תעבודדדד!") // הטקסט שיופיע
		);
	}
	if (EquippedWeapon)
	{
		// כאן אנחנו קוראים לפונקציה של הנשק (זאת שהייתה ב-Comment)
		EquippedWeapon->SetWeaponCollisionEnabled(ECollisionEnabled::QueryOnly);
	}
}

void ASlashCharacter::DisableWeaponCollision()
{
	
	if (EquippedWeapon)
	{
		EquippedWeapon->SetWeaponCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}


void ASlashCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(FName("MoveForward"), this, &ASlashCharacter::MoveForward);
	PlayerInputComponent->BindAxis(FName("MoveRight"), this, &ASlashCharacter::MoveRight);
	PlayerInputComponent->BindAxis(FName("Turn"), this, &ASlashCharacter::Turn);
	PlayerInputComponent->BindAxis(FName("LookUp"), this, &ASlashCharacter::LookUp);

	PlayerInputComponent->BindAction(FName("Jump"), IE_Pressed, this, &ACharacter::Jump);
	//חרב
	PlayerInputComponent->BindAction(FName("Equip"), IE_Pressed, this, &ASlashCharacter::EKeyPressed);
	//חיבור הלחיצה השמאלית לפונקצית המונטז
	PlayerInputComponent->BindAction(FName("Attack"), IE_Pressed, this, &ASlashCharacter::Attack);
}
