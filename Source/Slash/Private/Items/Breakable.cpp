// #include "Items/Breakable.h"
// #include "GeometryCollection/GeometryCollectionComponent.h"
// #include "Items/Treasure.h" 
// #include "Components/CapsuleComponent.h"
//
//
//
//
// ABreakable::ABreakable()
// {
// 	PrimaryActorTick.bCanEverTick = false;
//
// 	// // יצירת רכיב השברים (Geometry Collection)
// 	// GeometryCollection = CreateDefaultSubobject<UGeometryCollectionComponent>(TEXT("GeometryCollection"));
// 	// SetRootComponent(GeometryCollection);
// 	//
// 	// // הגדרות קוליזציה לשברים
// 	// GeometryCollection->SetGenerateOverlapEvents(true);
// 	// GeometryCollection->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
// 	// GeometryCollection->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
// 	//
// 	// // יצירת הקפסולה לחסימת השחקן
// 	// Capsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule"));
// 	// Capsule->SetupAttachment(GetRootComponent());
// 	// Capsule->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
// 	// Capsule->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Block);
// 	
// 	
// 	GeometryCollection = CreateDefaultSubobject<UGeometryCollectionComponent>(TEXT("GeometryCollection"));
// 	SetRootComponent(GeometryCollection);
// 	
// 	// יצירת הקפסולה וחיבורה לשורש
// 	Capsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule"));
// 	Capsule->SetupAttachment(GetRootComponent());
//
// 	// הגדרות Collision: להתעלם מהכל חוץ מהשחקן
// 	Capsule->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
// 	Capsule->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Block);
//
// 	// הגדרת ה-GeometryCollection (הכד עצמו) שיתעלם מהשחקן ומהמצלמה
// 	GeometryCollection->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
// 	GeometryCollection->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
// 	
// }
//
// void ABreakable::BeginPlay()
// {
// 	Super::BeginPlay();
// }
//
//
// void ABreakable::GetHit_Implementation(const FVector& ImpactPoint)
// {
// 	
// 	// 1. ביטול החסימה של הקפסולה כדי שהשחקן יוכל לעבור דרך השברים
// 	Capsule->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
//
// 	// 2. יצירת האוצר (Spawn)
// 	UWorld* World = GetWorld();
// 	if (World && TreasureClass)
// 	{
// 		FVector Location = GetActorLocation();
// 		Location.Z += 75.f; // הגבהה כדי שלא ייתקע ברצפה
//
// 		World->SpawnActor<ATreasure>(TreasureClass, Location, GetActorRotation());
// 	}
// 	
// 	if (World && TreasureClass)
//     {
//         UE_LOG(LogTemp, Warning, TEXT("Spawning Treasure!")); // שורת בדיקה
//         FVector Location = GetActorLocation();
//         Location.Z += 75.f;
//         World->SpawnActor<ATreasure>(TreasureClass, Location, GetActorRotation());
//     }
//     else
//     {
//         UE_LOG(LogTemp, Error, TEXT("TreasureClass is NULL or World is missing!"));
//     }
// }

// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Breakable.h"
#include "GeometryCollection/GeometryCollectionComponent.h"
#include "Items/Treasure.h" 
#include "Components/CapsuleComponent.h"


// Sets default values
ABreakableActor::ABreakableActor()
{
	PrimaryActorTick.bCanEverTick = false;
	GeometryCollection = CreateDefaultSubobject<UGeometryCollectionComponent>(TEXT("GeomtryCollection"));
	SetRootComponent(GeometryCollection);
	GeometryCollection->SetGenerateOverlapEvents(true);
	GeometryCollection->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	
	Capsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule"));
	Capsule->SetupAttachment(GetRootComponent());
	Capsule->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	Capsule->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Block);
}

void ABreakableActor::BeginPlay()
{
	Super::BeginPlay();
    
}

void ABreakableActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABreakableActor::GetHit_Implementation(const FVector& ImpactPoint)

{
	
	UE_LOG(LogTemp, Warning, TEXT("GetHit Called!"));
	
	UWorld* World = GetWorld();
	if (World && TreasureClasses.Num() > 2) {
		FVector Location = GetActorLocation();
		Location.Z += 75.f;
		
		int32 Selection = FMath::RandRange(0, TreasureClasses.Num() - 1);
		
		World->SpawnActor<ATreasure>(TreasureClasses[Selection], Location, GetActorRotation());
		
	}
}
