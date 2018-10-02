// Fill out your copyright notice in the Description page of Project Settings.

#include "OpenDoor.h"


// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();
	ActorThatOpens = GetWorld()->GetFirstPlayerController()->GetPawn();
	this->Owner = GetOwner();
}

void UOpenDoor::OpenDoor() 
{
	this->Owner->SetActorRotation(FRotator(0.0f, 0.0f, 0.0f));
}

void UOpenDoor::CloseDoor() 
{
	this->Owner->SetActorRotation(FRotator(0.0f, this->CloseAngle, 0.0f));
}

// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) {
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Poll the trigger volume.
	if (this->PressurePlate->IsOverlappingActor(ActorThatOpens)) {
		this->OpenDoor();
		this->LastDoorOpenTime = GetWorld()->GetTimeSeconds();
	}

	// Close door after 1 sec of not standing on P plate.
	if ((GetWorld()->GetTimeSeconds() - this->LastDoorOpenTime) >= this->DoorCloseDelay)
	{
		this->CloseDoor();
	}
}

