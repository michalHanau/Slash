#include "Items/Breakable.h"
#include "GeometryCollection/GeometryCollectionComponent.h"
#include "Items/Treasure.h" 
#include "Components/CapsuleComponent.h"
#include "Field/FieldSystemComponent.h"



ABreakable::ABreakable()
{
	PrimaryActorTick.bCanEverTick = false;

	// יצירת רכיב השברים (Geometry Collection)
	GeometryCollection = CreateDefaultSubobject<UGeometryCollectionComponent>(TEXT("GeometryCollection"));
	SetRootComponent(GeometryCollection);

	// הגדרות קוליזציה לשברים
	GeometryCollection->SetGenerateOverlapEvents(true);
	GeometryCollection->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	GeometryCollection->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);

	// יצירת הקפסולה לחסימת השחקן
	Capsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule"));
	Capsule->SetupAttachment(GetRootComponent());
	Capsule->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	Capsule->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Block);
}

void ABreakable::BeginPlay()
{
	Super::BeginPlay();
}


void ABreakable::GetHit_Implementation(const FVector& ImpactPoint)
{
	// 1. ספאון האוצר (הקוד שלך מצוין)
	if (GetWorld() && TreasureClass)
	{
		FVector Location = GetActorLocation();
		Location.Z += 75.f;
		GetWorld()->SpawnActor<ATreasure>(TreasureClass, Location, GetActorRotation());
	}

	// 2. קריאה ל-Blueprint לטיפול בפיזיקה
	CreateFields(ImpactPoint);

	// 3. ביטול קוליזציה לקפסולה
	if (Capsule)
	{
		Capsule->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
	}
}