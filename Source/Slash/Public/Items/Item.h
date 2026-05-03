#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Item.generated.h"

class USphereComponent;

enum class EItemState : uint8
{
	EIS_Hovering, // מרחף בעולם
	EIS_Equipped  // מחובר לשחקן
};

UCLASS()
class SLASH_API AItem : public AActor
{
	GENERATED_BODY()
	
public:	
	AItem();
	virtual void Tick(float DeltaTime) override;
	
	float TransformedSin();
	
	// פונקציה שרק מחזירה את ה-Mesh
	FORCEINLINE UStaticMeshComponent* GetMesh() const { return ItemMesh; }
	
	// פונקציה שרק מחזירה את ה-Sphere
	FORCEINLINE USphereComponent* GetSphere() const { return Sphere; }

protected:
	virtual void BeginPlay() override;
	
	UFUNCTION()
	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	virtual void OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
	EItemState ItemState = EItemState::EIS_Hovering; // ברירת מחדל - מרחף
	
	UPROPERTY(EditAnywhere)
	class UNiagaraComponent* EmbersEffect;
	
private:
	float RunningTime;
	
	
	UPROPERTY(EditAnywhere)
	float Amplitude = 0.25f;

	UPROPERTY(EditAnywhere)
	float ConstatTime = 5.f;
	
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* ItemMesh;
	
	UPROPERTY(VisibleAnywhere)
	USphereComponent * Sphere;
	

	
};
