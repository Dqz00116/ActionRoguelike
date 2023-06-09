// Fill out your copyright notice in the Description page of Project Settings.


#include "SInteractionComponent.h"

#include "DrawDebugHelpers.h"
#include "SCharacter.h"
#include "SGameplayInterface.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetSystemLibrary.h"


// Sets default values for this component's properties
USInteractionComponent::USInteractionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void USInteractionComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void USInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void USInteractionComponent::PrimaryInteract()
{
	FCollisionObjectQueryParams ObjectQueryParams;
	ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldDynamic);

	const auto& MyOwner = GetOwner();

	FVector EyeLocation;
	FRotator EyeRotation;
	MyOwner->GetActorEyesViewPoint(EyeLocation, EyeRotation);

	const auto& End = EyeLocation + (EyeRotation.Vector() * 1000);

	// FHitResult Hit;
	// bool bBlockingHit =  GetWorld()->LineTraceSingleByObjectType(Hit, EyeLocation, End, ObjectQueryParams);
	// const auto& HitActor = Hit.GetActor();
	//
	// if(HitActor!=nullptr)
	// {
	// 	const auto& bImplement = HitActor->Implements<USGameplayInterface>();
	// 	if(bImplement)
	// 	{
	// 		const auto& MyPawn = Cast<APawn>(MyOwner);
	// 		ISGameplayInterface::Execute_Interact(HitActor, MyPawn);
	// 	}
	// }
	
	FCollisionShape Shape;
	float Radius = 30.f;
	Shape.SetSphere(Radius);
	
	TArray<FHitResult> Hits;
	bool bBlockingHit = GetWorld()->SweepMultiByObjectType(Hits, EyeLocation, End, FQuat::Identity, ObjectQueryParams, Shape);
	FColor LineColor = bBlockingHit ? FColor::Green : FColor::Red;

	for(auto Hit : Hits)
	{
		const auto& HitActor = Hit.GetActor();
		if(HitActor!=nullptr)
		{
			const auto& bImplement = HitActor->Implements<USGameplayInterface>();
			if(bImplement)
			{
				const auto& MyPawn = Cast<APawn>(MyOwner);
				ISGameplayInterface::Execute_Interact(HitActor, MyPawn);
				break;
			}
		}
		DrawDebugSphere(GetWorld(), Hit.ImpactPoint, Radius, 32, LineColor, false, 2.f);
	}
	
	DrawDebugLine(GetWorld(), EyeLocation, End, LineColor, false, 2.f, 0, 2.f);

}

void USInteractionComponent::PrimaryDash()
{
	
	
	
}

void USInteractionComponent::PrimaryAttack(TSubclassOf<AActor> ProjectileClass)
{
	const auto& MyOwner = Cast<ASCharacter>(GetOwner());
	if(MyOwner)
	{
		const auto& HandLocation =  MyOwner->GetMesh()->GetSocketLocation(TEXT("Muzzle_01"));
		UCameraComponent* CameraComp =Cast<UCameraComponent>(MyOwner->GetComponentByClass(UCameraComponent::StaticClass()));

		FVector TrackLineStartLoc = CameraComp->GetComponentLocation();
		FVector TrackLineVec = CameraComp->GetComponentRotation().Vector();

		FVector TrackLineEndLoc = TrackLineStartLoc + TrackLineVec * 114514114514;
		FHitResult HitResult;
		TArray<AActor*> IgnoreActors;

		bool bIsHit = UKismetSystemLibrary::LineTraceSingle(GetWorld(), TrackLineStartLoc, TrackLineEndLoc, TraceTypeQuery1, false, IgnoreActors, EDrawDebugTrace::ForDuration, HitResult, true);

		FRotator TargetRot;
		FVector TargetVec;

		TargetVec = ( bIsHit ? HitResult.Location : TrackLineEndLoc) - HandLocation;
		TargetRot = TargetVec.Rotation();

		const auto& SpawnTM = FTransform(TargetRot, HandLocation);

		FActorSpawnParameters SpawnParameters;
		SpawnParameters.Instigator = MyOwner;
		SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	
		GetWorld()->SpawnActor<AActor>(ProjectileClass, SpawnTM, SpawnParameters);
	}
	
}
