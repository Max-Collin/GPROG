// Fill out your copyright notice in the Description page of Project Settings.


#include "Singletons/GameSingleton.h"

#include "DungeonGenerator.h"
#include "Engine/Engine.h"
#include "Kismet/GameplayStatics.h"

#include "HAL/PlatformFileManager.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"


UGameSingleton* UGameSingleton::GetInstance()
{
	
	if(GEngine)
	{
		
		UGameSingleton* Instance = Cast<UGameSingleton>(GEngine->GameSingleton);
		if(Instance)
		{
			return Instance;
		}
		else
		{
			Instance = NewObject<UGameSingleton>();
			GEngine->GameSingleton = Instance;
			
			return Instance;
		}
		
	}
	else
	{
		
		return nullptr;
	}
}

void UGameSingleton::SetTreasure()
{
	MaxTreasure++;
	UE_LOG(LogTemp,Warning,TEXT("Treasure Added; Trease now %d"),MaxTreasure)
}

void UGameSingleton::AddTreasure(AActor* Treasure)
{

	TreasureCollected++;
	UE_LOG(LogTemp,Warning,TEXT("Treasure Collected; Trease now %d"),TreasureCollected)
	if(TreasureCollected >= MaxTreasure)
	{
		bAllTreasure = true;
		NextLevel();	
	}
}

void UGameSingleton::SetEnemies()
{
	TotalEnemies++;
}

void UGameSingleton::KilledEnemy()
{
	SlainEnemies++;
	if(SlainEnemies>=TotalEnemies)
	{
		bAllEnemies =true;
		NextLevel();
	}
}

void UGameSingleton::ResetGameValues()
{
	Score=Score+TreasureCollected;
	MaxTreasure=0;
	TreasureCollected=0;
	SlainEnemies = 0;
	TotalEnemies = 0;
	bAllTreasure=false;
	bAllEnemies=false;
	
}

void UGameSingleton::ResetScore()
{
	Score=0;
}

void UGameSingleton::StoreScore()
{
	FString FileName = "ScoreFile.txt";
	FString FileLocation = FPaths::ProjectDir() + FileName;
	FString TextToWrite = "\n ";
	TextToWrite.AppendInt(Score);

	FFileHelper::SaveStringToFile(TextToWrite, *FileLocation,FFileHelper::EEncodingOptions::AutoDetect, &IFileManager::Get(),EFileWrite::FILEWRITE_Append);
	
		
	


}

void UGameSingleton::NewSeed(AActor* WorldContext)
{
	
	
}

void UGameSingleton::IterateCurrentLevel()
{
	CurrentLevel++;
	UE_LOG(LogTemp,Warning,TEXT("current level %d"), CurrentLevel);
}

void UGameSingleton::SetDungeonGenerator(ADungeonGenerator* NewDungeon)
{
	DungeonGenerator = NewDungeon;

	
}

void UGameSingleton::initalValuesCheck()
{
	if(MaxTreasure==0)
	{
		bAllTreasure = true;
	}
	if(TotalEnemies==0)
	{
		bAllEnemies = true;
	}
}

void UGameSingleton::NextLevel()
{
	if(bAllEnemies&&bAllTreasure)
	{
		if(OnAllTreasure.IsBound())
		{
			OnAllTreasure.Broadcast();
		}
	}
}
