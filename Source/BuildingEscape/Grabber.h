// Fill out your copyright notice in the Description page of Project Settings.

#pragma once



#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "Components/InputComponent.h"
#include "tuple"
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
    float reach = 200.f;
    UPhysicsHandleComponent* physicsHandle = nullptr;
    UInputComponent* inputComponent = nullptr;
    
 protected:
    FVector reachVector()const;
    void grab();
    void ungrab();
    void findPhysicshandle();
    void findInputComponent();
    std::tuple<FVector, FRotator> playerLocation() const;
    FHitResult rayCast() const;
};
