// Fill out your copyright notice in the Description page of Project Settings.


#include "Spawner.h"

#include "DungeonGenerator.h"
#include "Characters/enemies/BaseEnemy.h"
#include "Items/BaseItem.h"
#include "Kismet/KismetMathLibrary.h"
#include "Singletons/GameSingleton.h"

// Sets default values for this component's properties
USpawner::USpawner()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void USpawner::BeginPlay()
{
	Super::BeginPlay();

	UGameSingleton* GameSingleton = UGameSingleton::GetInstance();
	if(GameSingleton)
	{
		GameSingleton->ResetGameValues();
		
	}

	

	DungeonGenerator = Cast<ADungeonGenerator>(GetOwner());
	Stream = DungeonGenerator->GetStream();
	Scale = DungeonGenerator->GetScale();
	if(DungeonGenerator)
	{
		SpawnLocationTiles = DungeonGenerator->GetFloorTiles();
	}	
	SpawnEndDoor();
	Spawn();
	
	
}

void USpawner::Spawn()
{
	

		if(!RoomOnly)
		{
			SpawnLocationTiles.Append(DungeonGenerator->GetCorridorTiles());
		}
		SpawnItems();
	
}
void USpawner::SpawnItems()
{
	
	int32 ItemsSpawned=0;
	int32 FloorIndex;
	while (ItemsSpawned<Quantity)
	{
		FloorIndex = Stream.RandRange(0,SpawnLocationTiles.Num()-1);
		FVector FloorIndexVector = UKismetMathLibrary::Conv_IntVectorToVector(SpawnLocationTiles[FloorIndex]);
		FVector TempVector = UKismetMathLibrary::Add_VectorVector(FloorIndexVector *Scale,DungeonGenerator->GetActorLocation());
		FVector SpawnLocationVector = UKismetMathLibrary::Add_VectorVector(TempVector,offset);
		TArray< TSubclassOf<AActor>> Keys;
		SpawnList.GetKeys(Keys);
		
		for (auto Key : Keys)
		{
			
			bool bSpawn = UKismetMathLibrary::RandomBoolWithWeightFromStream(Stream,*SpawnList.Find(Key));
			if(bSpawn&&GetWorld())
			{
				
				AActor* SpawnedActor = GetWorld()->SpawnActor<AActor>(Key,SpawnLocationVector,FRotator(0.f,0.f,0.f));

				if(ABaseItem* Item = Cast<ABaseItem>(SpawnedActor))
				{
					
					UGameSingleton* GameSingleton = UGameSingleton::GetInstance();
					if(GameSingleton)
					{
						GameSingleton->SetTreasure();
						
					}
				}
				else if (ABaseEnemy* Enemy = Cast<ABaseEnemy>(SpawnedActor))
				{
					UGameSingleton* GameSingleton = UGameSingleton::GetInstance();
					if(GameSingleton)
					{
						GameSingleton->SetEnemies();
						
					}
				}
				SpawnLocationTiles.RemoveAt(FloorIndex);
				ItemsSpawned++;
				break;
			}
		}
	}

	UGameSingleton* GameSingleton = UGameSingleton::GetInstance();
	if(GameSingleton)
	{
		GameSingleton->initalValuesCheck();
						
	}
}

void USpawner::SpawnEndDoor()
{


	FVector FloorIndexVector = UKismetMathLibrary::Conv_IntVectorToVector(SpawnLocationTiles[SpawnLocationTiles.Num()-1]);
	FVector TempVector = UKismetMathLibrary::Add_VectorVector(FloorIndexVector *Scale,DungeonGenerator->GetActorLocation());
	FVector SpawnLocationVector = UKismetMathLibrary::Add_VectorVector(TempVector,offset);

	GetWorld()->SpawnActor<AActor>(EndDoor,SpawnLocationVector,FRotator(0.f,0.f,0.f));
	SpawnLocationTiles.RemoveAt(SpawnLocationTiles.Num()-1);
}


// Called every frame
void USpawner::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void USpawner::NewLevel()
{
	if(DungeonGenerator)
	{
		SpawnLocationTiles = DungeonGenerator->GetFloorTiles();
	}	
	SpawnEndDoor();
	Spawn();
}

