// Fill out your copyright notice in the Description page of Project Settings.

#include "OpenDoor.h"


// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
  PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UOpenDoor::BeginPlay()
{
  Super::BeginPlay();
  //    actorToTrigger = GetWorld()->GetFirstPlayerController()->GetPawn();
}

float UOpenDoor::getTotalMassOnTrigger() const
{
  TArray<AActor*> overlappedActors;
  TSubclassOf<AActor*> classFilter;
  trigger->GetOverlappingActors(OUT overlappedActors);
  float sumMass = 0.0f;
  for (const auto &actor : overlappedActors) {
    auto component = actor->FindComponentByClass<UPrimitiveComponent>();
    sumMass += component->GetMass();
  }
  return sumMass;
}

// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
  Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
  auto sumMass = getTotalMassOnTrigger();
  UE_LOG(LogTemp, Warning, TEXT("Mass isL %f"), sumMass);
  if (sumMass > 30.f){
    isOpened = true;
    OpenDoor();
  }else if (isOpened && (GetWorld()->GetTimeSeconds() - lastDoorOpenSeconds > openDoorDelay)){
    CloseDoor();
    isOpened = false;
  }
}

void UOpenDoor::OpenDoor(){
  auto rotator = FRotator(0.0f, -80.0f, 0.0f);
  GetOwner()->SetActorRotation(rotator);
  lastDoorOpenSeconds = GetWorld()->GetTimeSeconds();
}

void UOpenDoor::CloseDoor(){
  auto rotator = FRotator(0.0f, 0.0f, 0.0f);
  GetOwner()->SetActorRotation(rotator);
}
