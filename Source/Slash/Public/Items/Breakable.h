#pragma once

#include "CoreMinimal.h"
#include "Field/FieldSystemComponent.h"
#include "GameFramework/Actor.h"
#include "Interface/HitInterface.h"
#include "Breakable.generated.h"

UCLASS()
class SLASH_API ABreakable : public AActor, public IHitInterface
{
	GENERATED_BODY()

public:
	ABreakable();
	
	// מימוש פונקציית המכה מה-Interface
	virtual void GetHit_Implementation(const FVector& ImpactPoint) override;

protected:
	virtual void BeginPlay() override;

	// רכיב השברים (Geometry Collection)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UGeometryCollectionComponent* GeometryCollection;
	
	// קפסולה לחסימת השחקן לפני השבירה
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UCapsuleComponent* Capsule;
	//
	// // הוסיפי את אלו תחת ה-Protected או ה-Private
	// UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	// UFieldSystemComponent* FieldSystem;
	//
	// // רכיבים ליצירת הכוח
	// UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	// class URadialFalloff* RadialFalloff;
	//
	// UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	// class URadialVector* RadialVector;
	
	// תחת ה-protected או ה-public
	UFUNCTION(BlueprintImplementableEvent)
	void CreateFields(const FVector& FieldLocation);

private:
	// משתנה לבחירת ה-Blueprint של האוצר ב-Editor
	// ה-Forward Declaration (class ATreasure) חוסך Include בתוך ה-Header
	UPROPERTY(EditAnywhere, Category = "Breakable Properties")
	TSubclassOf<class ATreasure> TreasureClass;
	
	
	
};