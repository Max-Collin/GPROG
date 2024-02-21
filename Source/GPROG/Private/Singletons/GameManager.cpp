// Fill out your copyright notice in the Description page of Project Settings.


#include "Singletons/GameManager.h"

void UGameManager::SetMaxTreasure()
{

	
}

void UGameManager::AddTreasure()
{

	TreasureCollected++;
	UE_LOG(LogTemp,Warning,TEXT("Treasure Collected; Trease now %d"),TreasureCollected)
}
