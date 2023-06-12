// Fill out your copyright notice in the Description page of Project Settings.


#include "SAttributeComponent.h"

#include "SCharacter.h"

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

	if(HealthNow<0)
	{
		GetOwner()->Destroy();
	}
		
	return true;
}

float USAttributeComponent::GetDamage()
{
	return 100.f - this->Health;
}


