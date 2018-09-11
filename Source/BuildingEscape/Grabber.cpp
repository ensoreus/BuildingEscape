// Fill out your copyright notice in the Description page of Project Settings.

#include "Grabber.h"
#include "DrawDebugHelpers.h"
#include <tuple>
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
    findPhysicshandle();
    findInputComponent();
   
}

void UGrabber::findPhysicshandle(){
  physicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
    if (physicsHandle == nullptr) {
      UE_LOG(LogTemp, Error, TEXT("handle is empty"));
    }else{
      UE_LOG(LogTemp, Warning, TEXT("handle is inited"));
    } 
}

void UGrabber::findInputComponent()
{
  inputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
    if (inputComponent == nullptr) {
      UE_LOG(LogTemp, Error, TEXT("input is empty"));
    }else{
      inputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::grab);
      inputComponent->BindAction("Grab", IE_Released, this, &UGrabber::ungrab);
    }
}

std::tuple<FVector,FRotator> UGrabber::playerLocation() const
{
    FVector playerPawnLocation;
    FRotator playerPawnRotation;
    GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
                                                               OUT playerPawnLocation,
                                                               OUT playerPawnRotation);
    return std::tuple<FVector, FRotator>(playerPawnLocation, playerPawnRotation);
}

FVector UGrabber::reachVector() const
{
  auto location = playerLocation();
  FVector playerPawnLocation = std::get<0>(location);
  FRotator playerPawnRotation = std::get<1>(location);
    
    auto line = playerPawnLocation + playerPawnRotation.Vector() * reach;
    return line;
}

FHitResult UGrabber::rayCast() const
{
    FHitResult hitResult;
    auto line = reachVector();
    auto playerpawnpos = playerLocation();
    auto playerPawnLocation = std::get<0>(playerpawnpos);
    FCollisionQueryParams traceparams(FName(TEXT("")), false, GetOwner());
    GetWorld()->LineTraceSingleByObjectType(OUT hitResult,
                                            playerPawnLocation,
                                            line,
                                            FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
                                            traceparams);
    return hitResult;
}

// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
    auto playerPawnLocation = playerLocation();
    auto line = reachVector();
    DrawDebugLine(GetWorld(), std::get<0>(playerPawnLocation), line, FColor(100.f, 180.f, 100.f, 1.f));
    if (physicsHandle->GrabbedComponent != nullptr) {
      physicsHandle->SetTargetLocation(line);
    }
}

void UGrabber::grab(){
  auto hitResult = rayCast();
    if (hitResult.GetActor() != nullptr) {
      UE_LOG(LogTemp, Warning, TEXT("Grabbed: %s"), *hitResult.GetActor()->GetName());
      auto line = reachVector();
      physicsHandle->GrabComponent(hitResult.GetComponent(),
                                   hitResult.BoneName,
                                   hitResult.GetComponent()->GetOwner()->GetActorLocation(),
                                   true
                                   );
    }
}

void UGrabber::ungrab(){
  physicsHandle->ReleaseComponent();
  UE_LOG(LogTemp, Warning, TEXT("UnGrabbed: "));
}

