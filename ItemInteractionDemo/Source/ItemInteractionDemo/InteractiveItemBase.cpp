// Ignacio Roca UE5 Item Interaction Demo

#include "InteractiveItemBase.h"

AInteractiveItemBase::AInteractiveItemBase()
{
	PrimaryActorTick.bCanEverTick = true;
}

bool AInteractiveItemBase::IsActionAvailable_Implementation()
{
	return false;
}

void AInteractiveItemBase::DispatchOnActionEvents()
{
	if (IsActionAvailable())
		OnAction.Broadcast();
}

void AInteractiveItemBase::BeginPlay()
{
	Super::BeginPlay();
}

void AInteractiveItemBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


