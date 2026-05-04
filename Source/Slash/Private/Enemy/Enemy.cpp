

#include "Enemy/Enemy.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "DrawDebugHelpers.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "AttributeComponent.h"
#include "HUD/HealthBarComponent.h"


AEnemy::AEnemy()
{
	PrimaryActorTick.bCanEverTick = true;

	// הגדרת סוג האובייקט ל-WorldDynamic (כדי שהחרב תזהה אותו)
	GetMesh()->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);

	// חסימת ערוץ ה-Visibility
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);

	// התעלמות מהמצלמה (כדי שהמצלמה לא תקפוץ כשהאויב קרוב)
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);

	// הפעלת אירועי חפיפה (Overlap)
	GetMesh()->SetGenerateOverlapEvents(true);
	
	Attributes = CreateDefaultSubobject<UAttributeComponent>(TEXT("Attributes"));
	
	HealthBarWidget = CreateDefaultSubobject<UHealthBarComponent>(TEXT("HealthBar"));
	
}

void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	
	if (HealthBarWidget)
	{
		HealthBarWidget->SetVisibility(false);
	}
	
}

void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (CombatTarget)
	{
		// חישוב המרחק בין האויב למטרה
		const double DistanceToTarget = (CombatTarget->GetActorLocation() - GetActorLocation()).Size();

		// אם המרחק גדול מהרדיוס שהגדרנו
		if (DistanceToTarget > CombatRadius)
		{
			if (HealthBarWidget) 
			{
				HealthBarWidget->SetVisibility(false);
			}
			CombatTarget = nullptr; // מאפסים את המטרה
		}
	}

}

void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AEnemy::GetHit_Implementation(const FVector& ImpactPoint)
{
	if (HealthBarWidget)
	{
		HealthBarWidget->SetVisibility(true);
	}
	
	if (Attributes && Attributes->IsAlive())
	{
		DirectionalHitReact(ImpactPoint);
;
	}
	else
	{
		Die();
	}
	
	// --- האפקטים תמיד יקרו (גם במוות וגם בפגיעה רגילה) ---
	if (HitSound)UGameplayStatics::PlaySoundAtLocation(this, HitSound, ImpactPoint);
	if (HitParticles)UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitParticles, ImpactPoint);
}


void AEnemy::Die()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && DeathMontage)
	{
		AnimInstance->Montage_Play(DeathMontage);

		// הגרלה רנדומלית של פוזה (0-5)
		const int32 Selection = FMath::RandRange(0, 5);
		FName SectionName;

		// עדכון ה-Enum וה-Section בהתאם להגרלה
		switch (Selection)
		{
		case 0: 
			SectionName = FName("Death1"); 
			DeathPose = EDeathPose::EDP_Death1;
			break;
		case 1: 
			SectionName = FName("Death2"); 
			DeathPose = EDeathPose::EDP_Death2;
			break;
		case 2: 
			SectionName = FName("Death3"); 
			DeathPose = EDeathPose::EDP_Death3;
			break;
		case 3: 
			SectionName = FName("Death4"); 
			DeathPose = EDeathPose::EDP_Death4;
			break;
		case 4: 
			SectionName = FName("Death5"); 
			DeathPose = EDeathPose::EDP_Death5;
			break;
		case 5: 
			SectionName = FName("Death6"); 
			DeathPose = EDeathPose::EDP_Death6;
			break;
		}

		AnimInstance->Montage_JumpToSection(SectionName, DeathMontage);
	}
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	if (HealthBarWidget)
	{
		HealthBarWidget->SetVisibility(false);
	}
	SetLifeSpan(3.f);
}


void AEnemy::DirectionalHitReact(const FVector& ImpactPoint)
{
	// Calculate the angle between the forward vector and the vector from the enemy to the hit point
	const FVector Forward = GetActorForwardVector();
	const FVector ImpactLowered(ImpactPoint.X, ImpactPoint.Y, GetActorLocation().Z);
	const FVector ToHit = (ImpactLowered - GetActorLocation()).GetSafeNormal();


	const double CosTheta = FVector::DotProduct(Forward, ToHit);
	double Theta = FMath::RadiansToDegrees(FMath::Acos(CosTheta));
	const FVector CrossProduct = FVector::CrossProduct(Forward, ToHit);
	
	if (CrossProduct.Z < 0)
	{
		Theta *= -1.f;
	}

	// Determine the section to play based on the angle
	FName SectionName("FromBack");
	if (Theta >= -45.f && Theta < 45.f) SectionName = FName("FromFront");
	else if (Theta >= -135.f && Theta < -45.f) SectionName = FName("FromLeft");
	else if (Theta >= 45.f && Theta < 135.f) SectionName = FName("FromRight");

	// הפעלת האנימציה של הפגיעה
	PlayHitReactMontage(SectionName);
}



//האויב מקשיב לנזק
float AEnemy::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator,
	AActor* DamageCauser)
{
	// 2. עדכון משתנה ה-Health הפנימי (דרך ה-AttributeComponent)
	if (Attributes && HealthBarWidget)
	{
		Attributes->ReceiveDamage(DamageAmount); // פונקציה שמחסירה חיים
		// 3. עדכון ה-UI עם האחוז החדש
		HealthBarWidget->SetHealthPercent(Attributes->GetHealthPercent());
	}
	
	//שמירת הדמות שנתנה את המכה
	if (EventInstigator)
	{
		CombatTarget = EventInstigator->GetPawn();
	}

	return DamageAmount;
}

//קריאה לשימוש של האויב באנימציות שיצרנו לו(קבלת משתנה לאיזה צד ללכת)
void AEnemy::PlayHitReactMontage(const FName& SectionName)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && HitReactMontage)
	{
		AnimInstance->Montage_Play(HitReactMontage);
		AnimInstance->Montage_JumpToSection(SectionName, HitReactMontage);
	}
}

