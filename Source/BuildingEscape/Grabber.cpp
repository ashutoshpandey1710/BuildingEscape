// Fill out your copyright notice in the Description page of Project Settings.

#include "Grabber.h"

#define OUT


// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();

	//UE_LOG(LogTemp, Log, TEXT("Grabber reporting for duty!"));
	this->World = GetWorld();
	this->PlayerController = this->World->GetFirstPlayerController();
}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) {
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	FVector ViewLocation;
	FRotator ViewRotation;

	this->PlayerController->GetPlayerViewPoint(ViewLocation, ViewRotation);

	//FString rotation = ViewRotation.ToString();
	//UE_LOG(LogTemp, Warning, TEXT("The player is looking at %s."), *rotation)

	FVector LineHead = ViewLocation + ViewRotation.Vector() * this->Reach;
	DrawDebugLine(GetWorld(), ViewLocation, LineHead, FColor(255, 0, 0), false, 0.0f, 0.0f, 10.0f);

	///Setup query parameters.
	FCollisionQueryParams TraceParams(FName(TEXT("")), false, GetOwner());

	///Line trace (AKA Ray-Cast) out to grabber.
	FHitResult hit;

	if (this->World->LineTraceSingleByObjectType(
		OUT hit,
		ViewLocation,
		LineHead,
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		TraceParams
	))
	{
		AActor* HitActor = hit.GetActor();

		UE_LOG(LogTemp, Warning, TEXT("Ray Hit object %s."), *(HitActor->GetName()))
	}

}

