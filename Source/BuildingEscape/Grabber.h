// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameFramework/PlayerController.h"
#include "Engine/World.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "Components/PrimitiveComponent.h"
#include "Grabber.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDINGESCAPE_API UGrabber : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGrabber();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	APlayerController* PlayerController = nullptr;
	UWorld* World = nullptr;
	UPhysicsHandleComponent* PhysicsHandle = nullptr;
	UInputComponent* InputComponent = nullptr;

	///Ray cast and grab what's in reach.
	void Grab();
	void Release();

	// Find attached physics component.
	void FindPhysicsHandleComponent();

	// Setup (assumed) attached input component.
	void SetupInputComponent();

	// Get the first PhysicsBody in reach.
	const FHitResult GetFirstPhysicsBodyInReach();

	FVector GetReachLineEnd();
	FVector GetReachLineStart();

	FHitResult GetFirstHitOfTrace(FVector& TraceStart, FVector &TraceEnd);

	UPROPERTY(EditAnywhere)
		float Reach = 100.0f;
};
