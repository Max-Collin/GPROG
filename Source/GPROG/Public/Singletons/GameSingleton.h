// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GameSingleton.generated.h"

/**
 * 
 */

class ADungeonGenerator;
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAllTreasure);

UCLASS()
class GPROG_API UGameSingleton : public UObject
{
	GENERATED_BODY()

public:
	static UGameSingleton* GetInstance();

	
	void SetTreasure();
	void AddTreasure(AActor* Treasure);

	void SetEnemies();
	void KilledEnemy();
	void ResetGameValues();
	void ResetScore();
	void StoreScore();
	void NewSeed(AActor* WorldContext);
	void IterateCurrentLevel();
	void SetDungeonGenerator(ADungeonGenerator* NewDungeon);
	void initalValuesCheck();
	FOnAllTreasure OnAllTreasure;
	TObjectPtr<ADungeonGenerator> DungeonGenerator;

	int32 Score;
protected:
	
	
private:
	void NextLevel();
	bool bAllTreasure=false;
	bool bAllEnemies=false;
	int32 TreasureCollected=0;
	int32 MaxTreasure=0;
	int32 CurrentLevel=0;
	int32 TotalEnemies=0;
    int32 SlainEnemies=0;
public:
	FORCEINLINE int GetLevel() const {return CurrentLevel;}
	
};
