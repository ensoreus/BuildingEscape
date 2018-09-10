// Fill out your copyright notice in the Description page of Project Settings.

#include "Grabber.h"
#include "DrawDebugHelpers.h"
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
    UE_LOG(LogTemp, Warning, TEXT("Grabber online!"));
    physicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
    if (physicsHandle == nullptr) {
      UE_LOG(LogTemp, Warning, TEXT("handle is empty"));
    }else{
      UE_LOG(LogTemp, Warning, TEXT("handle is inited"));
    }
    
    inputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
    if (inputComponent == nullptr) {
      UE_LOG(LogTemp, Error, TEXT("input is empty"));
    }else{
      inputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::grab);
      inputComponent->BindAction("Grab", IE_Released, this, &UGrabber::ungrab);
    }
}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
    FVector playerPawnLocation;
    FRotator playerPawnRotation;
    GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
                                                               OUT playerPawnLocation,
                                                               OUT playerPawnRotation);
    
    auto line = playerPawnLocation + playerPawnRotation.Vector() * reach;
    DrawDebugLine(GetWorld(), playerPawnLocation, line, FColor(100.f, 180.f, 100.f, 1.f));
    
    
      
    
}

void UGrabber::grab(){
  FCollisionQueryParams traceparams(FName(TEXT("")), false, GetOwner());
   FVector playerPawnLocation;
    FRotator playerPawnRotation;
    GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
                                                               OUT playerPawnLocation,
                                                               OUT playerPawnRotation);
    
    auto line = playerPawnLocation + playerPawnRotation.Vector() * reach;
    FHitResult hitResult;
    GetWorld()->LineTraceSingleByObjectType(OUT hitResult,
                                            playerPawnLocation,
                                            line,
                                            FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
                                            traceparams);
    if (hitResult.GetActor() != nullptr) {
      UE_LOG(LogTemp, Warning, TEXT("Grabbed: %s"), *hitResult.GetActor()->GetName());
      physicsHandle->GrabComponent(hitResult.GetComponent(),
                                   hitResult.BoneName,
                                   line,
                                   false
                                   );
    }
    
}

void UGrabber::ungrab(){
  
  UE_LOG(LogTemp, Warning, TEXT("UnGrabbed: "));
}

