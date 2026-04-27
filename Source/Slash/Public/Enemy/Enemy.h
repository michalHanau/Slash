// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Interface/HitInterface.h"
#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Enemy.generated.h"

class UAnimMontage;

UCLASS()
class SLASH_API AEnemy : public ACharacter, public IHitInterface
{
	GENERATED_BODY()

public:
	AEnemy();
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//מימוש הפונקציה של הInterface
	virtual void GetHit(const FVector& ImpactPoint) override;

protected:
	virtual void BeginPlay() override;
	
	// הפונקציה מקבלת את שם הפרק ב-Montage שנרצה לנגן
	void PlayHitReactMontage(const FName& SectionName);
	
private:
	UPROPERTY(EditDefaultsOnly, Category = "Visuals")
	UAnimMontage* HitReactMontage;
	
	//הClass לפני חוסך את הINCLUDE
	UPROPERTY(EditAnywhere, Category = "Visual Effects")
	class UParticleSystem* HitParticles; // האפקט של הדם

	UPROPERTY(EditAnywhere, Category = "Sounds")
	class USoundBase* HitSound; // הסאונד של הפגיעה
};
