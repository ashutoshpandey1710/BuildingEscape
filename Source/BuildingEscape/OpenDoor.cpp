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
	this->Owner = GetOwner();

	if (!this->PressurePlate) {
		UE_LOG(LogTemp, Error, TEXT("No pressure plate attached to door %s!"), *(this->GetOwner()->GetName()));
	}
}

// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) {
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Poll the trigger volume.
	if (GetTotalMassOfActorsOnPlate() > this->TriggerMass) {
		this->OnOpen.Broadcast();
	}
	else{
		this->OnClose.Broadcast();
	}
}

float UOpenDoor::GetTotalMassOfActorsOnPlate(void) {
	float TotalMass = 0.0f;

	//Find all overlapping actors
	// Iterate over them to get their total mass.

	TArray<AActor*> OverlappingActors;

	if (!this->PressurePlate) { return TotalMass; }

	this->PressurePlate->GetOverlappingActors(OUT OverlappingActors);

	for (int i = 0; i < OverlappingActors.Num(); i++) {
		TotalMass += OverlappingActors[i]->FindComponentByClass<UPrimitiveComponent>()->GetMass();
		UE_LOG(LogTemp, Warning, TEXT("%s on pressure plate. Total Mass: %2.1f"), *(OverlappingActors[i]->GetName()), TotalMass)
	}

	return TotalMass;
}

