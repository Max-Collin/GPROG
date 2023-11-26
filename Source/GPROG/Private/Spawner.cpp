// Fill out your copyright notice in the Description page of Project Settings.


#include "Spawner.h"

#include "DungeonGenerator.h"
#include "Kismet/KismetMathLibrary.h"

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

	DungeonGenerator = Cast<ADungeonGenerator>(GetOwner());
	Stream = DungeonGenerator->GetStream();
	Scale = DungeonGenerator->GetScale();
	Spawn();
	// ...
	
}

void USpawner::Spawn()
{
	
	if(DungeonGenerator)
	{
		SpawnLocationTiles = DungeonGenerator->GetFloorTiles();
		if(!RoomOnly)
		{
			SpawnLocationTiles.Append(DungeonGenerator->GetCorridorTiles());
		}
		SpawnItems();
	}
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
		UE_LOG(LogTemp,Warning,TEXT("%d"),Keys.Num())
		for (auto Key : Keys)
		{
			UE_LOG(LogTemp,Warning,TEXT("In Loop"))
			bool bSpawn = UKismetMathLibrary::RandomBoolWithWeightFromStream(Stream,*SpawnList.Find(Key));
			if(bSpawn&&GetWorld())
			{
				
				GetWorld()->SpawnActor<AActor>(Key,SpawnLocationVector,FRotator(0.f,0.f,0.f));
				SpawnLocationTiles.RemoveAt(FloorIndex);
				ItemsSpawned++;
				break;
			}
		}
	}

	
}


// Called every frame
void USpawner::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

