

#include "Enemy/Enemy.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "DrawDebugHelpers.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"

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
}

void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	
}

void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AEnemy::GetHit_Implementation(const FVector& ImpactPoint)
{
	//חישוב הוקטור של הפגיעה
	const FVector Forward = GetActorForwardVector();
	FVector ImpactLowered(ImpactPoint.X, ImpactPoint.Y, GetActorLocation().Z);
	const FVector ToHit = (ImpactLowered - GetActorLocation()).GetSafeNormal();
	
	// חישוב הזווית של הפגיעה
	double CosTheta = FVector::DotProduct(Forward, ToHit);
	double Theta = FMath::Acos(CosTheta);
	Theta = FMath::RadiansToDegrees(Theta);
	
	
	// 7. ציור חץ אדום שמראה לאן האויב מסתכל (Forward)
	UKismetSystemLibrary::DrawDebugArrow(this, GetActorLocation(), GetActorLocation() + Forward * 60.f, 5.f, FColor::Red, 5.f);
	// 8. ציור חץ ירוק שמראה את כיוון המכה (ToHit)
	UKismetSystemLibrary::DrawDebugArrow(this, GetActorLocation(), GetActorLocation() + ToHit * 60.f, 5.f, FColor::Green, 5.f);
	// 9. הדפסת הזווית על המסך בצד שמאל למעלה
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(1, 5.f, FColor::Green, FString::Printf(TEXT("Theta: %f"), Theta));
	}
	
	
	//  חישוב ה-Cross Product
	const FVector CrossProduct = FVector::CrossProduct(Forward, ToHit);

	//  האם הווקטור מצביע למטה?
	if (CrossProduct.Z < 0)
	{
		// אם כן, סימן שהמכה באה משמאל - נהפוך את Theta למספר שלילי
		Theta *= -1.f;
	}

	// 3. ציור חץ לדיבאג (כדי שנראה את ה-Cross Product בעצמנו)
	// נצייר חץ כחול שיוצא ממרכז האויב למעלה או למטה
	UKismetSystemLibrary::DrawDebugArrow(this, GetActorLocation(), GetActorLocation() + CrossProduct * 100.f, 5.f, FColor::Blue, 5.f);

	// 4. הדפסת ה-Theta המעודכן
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(1, 5.f, FColor::Green, FString::Printf(TEXT("Theta: %f"), Theta));
	}
	
	//בדיקה מאיפה הגיעה המכה ושליחה להפעלת האנימציה המתאימה
	FName SectionName("FromBack");
	if (Theta >= -45.f && Theta < 45.f)
	{
		SectionName = FName("FromFront");
	}
	else if (Theta >= -135.f && Theta < -45.f)
	{
		SectionName = FName("FromLeft");
	}
	else if (Theta >= 45.f && Theta < 135.f)
	{
		SectionName = FName("FromRight");
	}
	PlayHitReactMontage(SectionName);
	
	// הפעלת סאונד בנקודת הפגיעה
	if (HitSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, HitSound, ImpactPoint);
	}
	//  יצירת אפקט דם בנקודת הפגיעה
	if (HitParticles)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitParticles, ImpactPoint);
	}
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

