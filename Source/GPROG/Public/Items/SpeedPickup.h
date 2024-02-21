// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/HeathPickup.h"
#include "SpeedPickup.generated.h"

/**
 * 
 */
UCLASS()
class GPROG_API ASpeedPickup : public AHeathPickup
{
	GENERATED_BODY()

public:
	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
    	                             UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
    	                             const FHitResult& SweepResult) override;
	
};
