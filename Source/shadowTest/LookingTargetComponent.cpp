// Fill out your copyright notice in the Description page of Project Settings.


#include "LookingTargetComponent.h"

// Sets default values for this component's properties
ULookingTargetComponent::ULookingTargetComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

}


// Called when the game starts
void ULookingTargetComponent::BeginPlay()
{
	Super::BeginPlay();
	
}


// Called every frame
void ULookingTargetComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (m_pTargetActor != nullptr)
	{
		AActor* owner = GetOwner();
		if (owner != nullptr)
		{
			// ‰ŠúÀ•W
			FVector start = owner->GetActorLocation();

			// Œü‚­•ûŒü
			FVector target = m_pTargetActor->GetActorLocation();

			FRotator lookRot = FRotationMatrix::MakeFromX(start - target).Rotator();

			owner->SetActorRotation(lookRot);
		}
		
	}
}

