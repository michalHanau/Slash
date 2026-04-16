#include "Items/Weapon.h"
#include "Characters/SlashCharacter.h" // חייב כדי להכיר את השחקן
#include "Components/SphereComponent.h"

void AWeapon::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnSphereOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);

	// לוגיקה ששומרת את החרב אצל השחקן כשעומדים לידה
	ASlashCharacter* SlashCharacter = Cast<ASlashCharacter>(OtherActor);
	if (SlashCharacter)
	{
		SlashCharacter->SetOverlappingWeapon(this);
	}
}

void AWeapon::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Super::OnSphereEndOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);

	// כשמתרחקים, השחקן כבר לא יכול להרים את החרב
	ASlashCharacter* SlashCharacter = Cast<ASlashCharacter>(OtherActor);
	if (SlashCharacter)
	{
		SlashCharacter->SetOverlappingWeapon(nullptr);
	}
}

// המימוש של החיבור ליד!
void AWeapon::Equip(USceneComponent* InParent, FName InSocketName)
{
	FAttachmentTransformRules TransformRules(EAttachmentRule::SnapToTarget, true);
	GetMesh()->AttachToComponent(InParent, TransformRules, InSocketName);
	
	if (GetSphere()) // Sphere הוא הרכיב שיוצר את הטווח מסביב לחרב
	{
		GetSphere()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

	if (GetMesh()) //ביטול ההתנגשות כשחרב ביד
	{
		GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		GetMesh()->SetCollisionResponseToAllChannels(ECR_Ignore);
	}
	
	//שינוי מצב בנשק
	ItemState = EItemState::EIS_Equipped;
}

