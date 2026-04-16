#include "Items/Item.h"
#include "Slash/DebugMacros.h"
#include "Components/SphereComponent.h"

AItem::AItem()
{
	PrimaryActorTick.bCanEverTick = true;

	ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemMeshComponent"));
	RootComponent = ItemMesh;
	
	Sphere = CreateDefaultSubobject<USphereComponent>("Sphere");
	Sphere->SetupAttachment(GetRootComponent());
}

void AItem::BeginPlay()
{
	Super::BeginPlay();
	
	Sphere->OnComponentBeginOverlap.AddDynamic(this, &AItem::OnSphereOverlap);
	Sphere->OnComponentEndOverlap.AddDynamic(this, &AItem::OnSphereEndOverlap);
}

void AItem::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
		const FString OtherActorName = OtherActor->GetName();
        
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(1, 30.f, FColor::Red, OtherActorName);
		}
}

void AItem::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
		const FString OtherActorName = FString("Ending overlap with: ") + OtherActor->GetName();

		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(1, 30.f, FColor::Blue, OtherActorName);
		}
}

float AItem::TransformedSin() {
	return Amplitude * FMath::Sin(RunningTime * ConstatTime);
}

void AItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	
	Super::Tick(DeltaTime);
	RunningTime += DeltaTime;
	float DeltaZ = TransformedSin();

	AddActorWorldOffset(FVector(0.f, 0.f,DeltaZ));
}

