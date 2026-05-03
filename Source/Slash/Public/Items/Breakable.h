// #pragma once
//
// #include "CoreMinimal.h"
// #include "Field/FieldSystemComponent.h"
// #include "GameFramework/Actor.h"
// #include "Interface/HitInterface.h"
// #include "Breakable.generated.h"
//
//
// UCLASS()
// class SLASH_API ABreakable : public AActor, public IHitInterface
// {
// 	GENERATED_BODY()
//
// public:
// 	ABreakable();
// 	
// 	// מימוש פונקציית המכה מה-Interface
// 	virtual void GetHit_Implementation(const FVector& ImpactPoint) override;
//
// protected:
// 	virtual void BeginPlay() override;
//
// 	// רכיב השברים (Geometry Collection)
// 	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
// 	class UGeometryCollectionComponent* GeometryCollection;
// 	
// 	// קפסולה לחסימת השחקן לפני השבירה
// 	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
// 	class UCapsuleComponent* Capsule;
// 	
// 	UPROPERTY(EditAnywhere, Category = "Breakable Properties")
// 	TSubclassOf<class ATreasure> TreasureClass;
//
// 	//
// 	// // הוסיפי את אלו תחת ה-Protected או ה-Private
// 	// UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
// 	// UFieldSystemComponent* FieldSystem;
// 	//
// 	// // רכיבים ליצירת הכוח
// 	// UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
// 	// class URadialFalloff* RadialFalloff;
// 	//
// 	// UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
// 	// class URadialVector* RadialVector;
// 	
// 	// תחת ה-protected או ה-public
// 	UFUNCTION(BlueprintImplementableEvent)
// 	void CreateFields(const FVector& FieldLocation);
// 	
//
// private:
//
// 	
// 	
// 	
// };


// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface/HitInterface.h"
#include "Breakable.generated.h"


class UGeometryCollectionComponent;
class UCapsuleComponent;

UCLASS()
class SLASH_API ABreakableActor : public AActor, public IHitInterface
{
	GENERATED_BODY()
    
public:    
	ABreakableActor();
	virtual void Tick(float DeltaTime) override;
	virtual void GetHit_Implementation(const FVector& ImpactPoint) override;

protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UCapsuleComponent* Capsule;

private:

	UPROPERTY(VisibleAnywhere)
	UGeometryCollectionComponent* GeometryCollection;
	
	UPROPERTY(EditAnywhere, Category = "Treasure Properties")
	TArray<TSubclassOf<class ATreasure>> TreasureClasses;
	
	

};