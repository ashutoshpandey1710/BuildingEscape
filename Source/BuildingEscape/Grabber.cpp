// Fill out your copyright notice in the Description page of Project Settings.

#include "Grabber.h"

#define OUT
#define IN


// Sets default values for this component's properties
UGrabber::UGrabber(){
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UGrabber::BeginPlay() {
	Super::BeginPlay();

	this->World = GetWorld();
	this->PlayerController = this->World->GetFirstPlayerController();
	this->FindPhysicsHandleComponent();
	this->SetupInputComponent();
}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) {
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	/// If a physics body has been grabbed, set it's target locatoin to the current view (at the end of your reach) ie. line head.
	if (this->PhysicsHandle->GrabbedComponent) {
		PhysicsHandle->SetTargetLocation(this->GetReachLineEnd());
	}
}

void UGrabber::FindPhysicsHandleComponent() {
	///Look for attached physics handle.
	this->PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (!this->PhysicsHandle) {	
		UE_LOG(LogTemp, Error, TEXT("Physics Handle is NOT attachded!"));
	}
}

///Looking for input component.
void UGrabber::SetupInputComponent() {
	this->InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	if (this->InputComponent) {
		UE_LOG(LogTemp, Warning, TEXT("Input Component is here! Rejoice! Name: %s."), *(this->InputComponent->GetName()));
		this->InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		this->InputComponent->BindAction("Grab", IE_Released, this, &UGrabber::Release);
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("Input Handle is NOT attachded!"));
	}
}



const FHitResult UGrabber::GetFirstPhysicsBodyInReach() {
	FHitResult hit;

	FCollisionQueryParams TraceParams(FName(TEXT("")), false, GetOwner());

	if (this->World->LineTraceSingleByObjectType(
		OUT hit,
		this->GetReachLineStart(),
		this->GetReachLineEnd(),
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		TraceParams
	)) {
		UE_LOG(LogTemp, Warning, TEXT("Ray Hit object %s."), *(hit.GetActor()->GetName()))
	}
	return hit;
}

FVector UGrabber::GetReachLineEnd() {
	FRotator ViewDirection;
	FVector ReachLineStart, ReachLineEnd;

	this->PlayerController->GetPlayerViewPoint(OUT ReachLineStart, OUT ViewDirection);

	return ReachLineStart + ViewDirection.Vector() * this->Reach;
}

FVector UGrabber::GetReachLineStart() {
	FRotator ViewDirection;
	FVector ReachLineStart, ReachLineEnd;

	this->PlayerController->GetPlayerViewPoint(ReachLineStart, ViewDirection);

	return ReachLineStart;
}

void UGrabber::Grab() {
	UE_LOG(LogTemp, Warning, TEXT("Grab Pressed!"));
	

	///Try and reach any actor with physics body collision set.
	auto Hit = GetFirstPhysicsBodyInReach();
	auto ComponentToGrab = Hit.GetComponent();
	auto ActorHit = Hit.GetActor();

	///If we hit something, attach a physics handle.

	if (ActorHit) {
		// Attach Physics handle.
		this->PhysicsHandle->GrabComponent(
			ComponentToGrab,
			NAME_None,
			ComponentToGrab->GetOwner()->GetActorLocation(),
			true // Allow rotation.
		);
	}

	
}

void UGrabber::Release() {
	UE_LOG(LogTemp, Warning, TEXT("Grab Released!"));
	//Release physics handle
	PhysicsHandle->ReleaseComponent();
}


