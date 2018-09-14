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
  auto iterator = TActorIterator<ATriggerVolume>(GetWorld());
  for (;iterator; ++iterator)
     {
       if (iterator->GetName() == "openDoorTriggerVolume"){
         UE_LOG(LogTemp, Warning, TEXT("found trigger!"));
         trigger = *iterator;
       }
     }
  //    actorToTrigger = GetWorld()->GetFirstPlayerController()->GetPawn();
  
}

float UOpenDoor::getTotalMassOnTrigger() const
{
  TArray<AActor*> overlappedActors;
  TSubclassOf<AActor*> classFilter;
  
  if (trigger == nullptr) {
    return 0.0f;
  }
  
  trigger->GetOverlappingActors(OUT overlappedActors);
  float sumMass = 0.0f;
    for (const auto *actor : overlappedActors) {
      auto component = actor->FindComponentByClass<UPrimitiveComponent>();
      if (component != nullptr) {
        sumMass += component->GetMass();        
      }    
    }
  return sumMass;
}

// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
  Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
  auto sumMass = getTotalMassOnTrigger();

  if (sumMass >= 30.f){
    OpenDoor();
  }else if (isOpened && (GetWorld()->GetTimeSeconds() - lastDoorOpenSeconds > openDoorDelay)){
    CloseDoor();

  }
}

void UOpenDoor::OpenDoor(){
  isOpened = true;
  onOpenRequest.Broadcast();
  lastDoorOpenSeconds = GetWorld()->GetTimeSeconds();
}

void UOpenDoor::CloseDoor(){
  onCloseRequest.Broadcast();
  isOpened = false;
}
