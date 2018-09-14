// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/TriggerVolume.h"
#include "Runtime/Engine/Public/EngineUtils.h"
#include "OpenDoor.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnOpenDoorEvent);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCloseDoorEvent);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDINGESCAPE_API UOpenDoor : public UActorComponent
{
  GENERATED_BODY()

    public:	
    // Sets default values for this component's properties
    UOpenDoor();

 protected:
  // Called when the game starts
  virtual void BeginPlay() override;
  virtual void OpenDoor();
  virtual void CloseDoor();
 private:
  float openAngle = 80.0f;      
  ATriggerVolume* trigger = nullptr;

  bool isOpened = false;
  float getTotalMassOnTrigger() const;
        
  UPROPERTY(EditAnywhere)
    float openDoorDelay = 1.f;

  UPROPERTY(BlueprintAssignable)
    FOnOpenDoorEvent onOpenRequest;
  UPROPERTY(BlueprintAssignable)
    FOnCloseDoorEvent onCloseRequest;
  
  float lastDoorOpenSeconds = 1.f;
 public:	
  // Called every frame
  virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
};
