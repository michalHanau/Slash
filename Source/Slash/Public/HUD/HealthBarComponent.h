// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "HealthBarComponent.generated.h"



UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SLASH_API UHealthBarComponent : public UWidgetComponent
{
	GENERATED_BODY()

public:	
	UHealthBarComponent();
	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	//הפונקציה שקוראים לה מהאויב
	void SetHealthPercent(float Percent);

protected:
	virtual void BeginPlay() override;

private:	
	// נשמור פה את ה-Widget כדי שלא נצטרך לעשות Cast כל פעם מחדש (ביצועים!)
	UPROPERTY()
	class UHealthBar* HealthBarWidget;
		
};
