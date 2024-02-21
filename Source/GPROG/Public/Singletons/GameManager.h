// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "GameManager.generated.h"

/**
 * 
 */
UCLASS()
class GPROG_API UGameManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()
public:
	void SetMaxTreasure();
	void AddTreasure();
protected:
private:
	int32 TreasureCollected=0;
	int32 MaxTreasure=0;
public:
	
};
