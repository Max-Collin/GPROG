// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DungeonGenerator.generated.h"


class UInstancedStaticMeshComponent;

UCLASS()
class GPROG_API ADungeonGenerator : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADungeonGenerator();
	
	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;



	
	
	UPROPERTY(EditAnywhere , Category = "Map Settings")
	int32 Seed;
	
	UPROPERTY(EditAnywhere , Category = "Map Settings")
	int32 RoomSize_Min=3;

	UPROPERTY(EditAnywhere , Category = "Map Settings")
	int32 RoomSize_Max=9;

	UPROPERTY(EditAnywhere , Category = "Map Settings")
	int32 RoomCount=1;
	
	UPROPERTY(EditAnywhere , Category = "Map Settings")
	int32 FloorCull_Min=0;

	UPROPERTY(EditAnywhere , Category = "Map Settings")
	int32 FloorCull_Max=1;

	UPROPERTY(EditAnywhere , Category = "Map Settings")
	bool bFloorCulling = false;

	UPROPERTY(EditAnywhere , Category = "Map Settings")
	int32 BranchingThreshold;

	UPROPERTY(EditAnywhere , Category = "Map Settings")
	float BranchingChance;

	UPROPERTY(EditAnywhere , Category = "Map Settings")
	bool Merging;
	
	UPROPERTY(EditAnywhere , Category = "Map Settings")
	bool Branching;


	UPROPERTY(EditAnywhere , Category = "Editor Settings")
	int32 MaxLoops;

	UPROPERTY(EditAnywhere , Category = "Editor Settings")
	float Scale;

	UPROPERTY(EditAnywhere , Category = "Editor Settings")
	bool NewSeed = true;
	
	UPROPERTY(EditAnywhere)
	FRandomStream Stream;

	//Meshes
	UPROPERTY(EditAnywhere,BlueprintReadWrite , Category = "Meshes ")
	UInstancedStaticMeshComponent* FloorMesh;

	


private:

	void GenerateMap();
	void Room_GenerateFloor(FIntVector RootLocation );
	void SpawnTiles();
	bool Room_FindNextLocation(FIntVector &NewLocation);
	FIntVector TestRelativeTileLocation(FIntVector ReferenceLocation,int32 X,int32 Y, bool &thisISFloorTile);
	TArray<FIntVector> FloorTiles;
	
	FIntVector MaxExtents;
	TMap<FIntVector,FIntVector> Rooms;
	FIntVector PreviousLocation = FIntVector(0,0,0);
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};