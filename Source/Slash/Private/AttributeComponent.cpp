// Fill out your copyright notice in the Description page of Project Settings.


#include "AttributeComponent.h"


// Sets default values for this component's properties
UAttributeComponent::UAttributeComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	Health = 100.f;
	MaxHealth = 100.f;
}


void UAttributeComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


void UAttributeComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UAttributeComponent::ReceiveDamage(float Damage)
{
	Health = FMath::Clamp(Health - Damage, 0.f, MaxHealth);
}

float UAttributeComponent::GetHealthPercent()
{
	return Health/MaxHealth;
}

bool UAttributeComponent::IsAlive() const
{
	return Health > 0.f; // מחזיר אמת אם הבריאות גדולה מ-0
}

