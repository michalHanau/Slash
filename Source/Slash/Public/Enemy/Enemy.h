// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Characters/CharacterTypes.h"
#include "Interface/HitInterface.h"
#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Enemy.generated.h"


class UAnimMontage;
class UAttributeComponent;
class UWidgetComponent;

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
	virtual void GetHit_Implementation(const FVector& ImpactPoint) override;
	
	//דריסת הפונקציה של אנריל כך שהאויב יקשיב לנזק
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

protected:
	virtual void BeginPlay() override;
	
	// הפונקציה מקבלת את שם הפרק ב-Montage שנרצה לנגן
	void PlayHitReactMontage(const FName& SectionName);
	
	//מחשבת את זווית הפגיעה
	void DirectionalHitReact(const FVector& ImpactPoint);
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attributes")
	EDeathPose DeathPose = EDeathPose::EDP_Alive;
	
private:
	UPROPERTY(EditDefaultsOnly, Category = "Visuals")
	UAnimMontage* HitReactMontage;
	
	//הClass לפני חוסך את הINCLUDE
	UPROPERTY(EditAnywhere, Category = "Visual Effects")
	class UParticleSystem* HitParticles; // האפקט של הדם

	UPROPERTY(EditAnywhere, Category = "Sounds")
	class USoundBase* HitSound; 
	
	UPROPERTY(VisibleAnywhere)
	UAttributeComponent* Attributes;
	
	UPROPERTY(VisibleAnywhere)
	class UHealthBarComponent* HealthBarWidget;
	
	// פונקציה לטיפול בלוגיקת המוות של האויב
	void Die();
	
	UPROPERTY(EditAnywhere, Category = "Visuals")
	UAnimMontage* DeathMontage;
	
	// המטרה שהאויב כרגע נלחם נגדה
	UPROPERTY(VisibleAnywhere)
	AActor* CombatTarget;

	// המרחק שמעבר לו המד ייעלם
	UPROPERTY(EditAnywhere)
	double CombatRadius = 500.f;
	

};
