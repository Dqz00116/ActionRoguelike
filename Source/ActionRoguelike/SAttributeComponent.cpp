// Fill out your copyright notice in the Description page of Project Settings.


#include "SAttributeComponent.h"

// Sets default values for this component's properties
USAttributeComponent::USAttributeComponent()
{
	this->Health = 100;
}


// Called when the game starts
void USAttributeComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


bool USAttributeComponent::ApplyHealthChange(float Delta)
{
	const auto& HealthNow = this->Health + Delta;
	this->Health = HealthNow<0 ?  0 : HealthNow;
	
	OnHealthChanged.Broadcast(nullptr, this, Health, Delta);
		
	return true;
}


