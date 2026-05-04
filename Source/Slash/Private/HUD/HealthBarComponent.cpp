// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/HealthBarComponent.h"
#include "HUD/HealthBar.h"
#include "Components/ProgressBar.h"
#include "Components/WidgetComponent.h"

UHealthBarComponent::UHealthBarComponent()
{
	PrimaryComponentTick.bCanEverTick = true;


}


void UHealthBarComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

void UHealthBarComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UHealthBarComponent::SetHealthPercent(float Percent)
{
	// אם זו הפעם הראשונה, נשיג את האובייקט ונעשה לו Cast
	if (HealthBarWidget == nullptr)
	{
		HealthBarWidget = Cast<UHealthBar>(GetUserWidgetObject());
	}

	// אם ה-Cast הצליח וה-ProgressBar קיים
	if (HealthBarWidget && HealthBarWidget->HealthBar)
	{
		HealthBarWidget->HealthBar->SetPercent(Percent);
	}
}

