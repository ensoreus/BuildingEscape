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
    actorToTrigger = GetWorld()->GetFirstPlayerController()->GetPawn();
}


// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
    if (trigger->IsOverlappingActor(actorToTrigger)){
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
