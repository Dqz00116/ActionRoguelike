// Fill out your copyright notice in the Description page of Project Settings.


#include "SItemChest.h"

#include "DrawDebugHelpers.h"
#include "Particles/ParticleSystemComponent.h"

void ASItemChest::Interact_Implementation(APawn* InstigatorPawn)
{
	ISGameplayInterface::Interact_Implementation(InstigatorPawn);
	if(bChestStatus)
	{
		// LidMesh->SetRelativeRotation(FRotator(0, 0, 0));
		bChestStatus = false;
	}
	else
	{
		// LidMesh->SetRelativeRotation(FRotator(TargetPitch, 0, 0));
		bChestStatus = true;
	}
	// OpenEffectComp->Activate(bChestStatus);

	check(GEngine != nullptr);
	FString DebugMessage = TEXT("bChestStatus:");
	DebugMessage.AppendInt(bChestStatus);
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, DebugMessage);
	
}

// Sets default values
ASItemChest::ASItemChest()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BaseMesh"));
	RootComponent = BaseMesh;

	LidMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LidMesh"));
	LidMesh->SetupAttachment(BaseMesh);

	GoodsMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("GoodsMesh"));
	GoodsMesh->SetupAttachment(BaseMesh);

	OpenEffectComp = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("EffectComp"));
	OpenEffectComp->SetupAttachment(GoodsMesh);
	OpenEffectComp->bAutoActivate = false;
	
	TargetPitch = 110;
	bChestStatus = false;
}

// Called when the game starts or when spawned
void ASItemChest::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASItemChest::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASItemChest::SetBChestStatus(bool status)
{
	this->bChestStatus = status; 
}

bool ASItemChest::GetBChestStatus()
{
	return this->bChestStatus;
}

