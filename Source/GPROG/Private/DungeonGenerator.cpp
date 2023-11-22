// Fill out your copyright notice in the Description page of Project Settings.



#include "GPROG/Public/DungeonGenerator.h"


#include "Kismet/KismetMathLibrary.h"
#include "Math/RandomStream.h"
#include "Components/InstancedStaticMeshComponent.h"

// Sets default values
ADungeonGenerator::ADungeonGenerator(): BranchingThreshold(0), BranchingChance(0), Merging(false), Branching(false),
                                        MaxLoops(0), Scale(0),
                                        MaxExtents()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	FloorMesh = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("Floor Mesh"));
	
}

// Called when the game starts or when spawned
void ADungeonGenerator::BeginPlay()
{
	Super::BeginPlay();
	NewSeed = false;
	Seed = FMath::Rand() % 999999999;
	Stream.Initialize(Seed);
	GenerateMap();
	
}

void ADungeonGenerator::GenerateMap()
{
	FIntVector NewLocation;
	for(int i =0 ; i<RoomCount;i++)
	{
		if(i==0)
		{
			Room_GenerateFloor(PreviousLocation);
			Rooms.Add(PreviousLocation,MaxExtents);
			
		}
		else
		{
			if(Room_FindNextLocation(NewLocation))
			{
				Room_GenerateFloor(NewLocation);
				Rooms.Add(NewLocation,MaxExtents);
				PreviousLocation = NewLocation;
				
			}
		}
		
	}
	SpawnTiles();
}

void ADungeonGenerator::Room_GenerateFloor(FIntVector RootLocation  )
{
	int32 Out_X = Stream.RandRange(RoomSize_Min,RoomSize_Max);
	int32 Out_Y = Stream.RandRange(RoomSize_Min,RoomSize_Max);
	TArray<int32> ExtentsX;
	TArray<int32> ExtentsY;
	int32 MaxExtents_X;
	int32 MaxExtents_Y;
	bool Working = true;
	TArray<FIntVector> Tiles;
	TArray<FIntVector> ConnectedTiles;

	
		for(int i =0;i<=Out_X*Out_Y-1;i++)
		{
			int32 Tile_X = (i/Out_Y)+RootLocation.X;
			int32 Tile_Y = (i%Out_Y)+RootLocation.Y;
			Tiles.Add(FIntVector(Tile_X,Tile_Y,RootLocation.Z));
			
		}
	ConnectedTiles = Tiles;
	if(bFloorCulling)
	{
		while (Working)
		{
			TArray<FIntVector> TilesCopy = Tiles;
			
			int32 LastIndex =FMath::Clamp( Stream.RandRange(FloorCull_Min,FloorCull_Max)-1,0,TilesCopy.Num()/4);
			for(int32 i=0;i<=LastIndex;i++)
			{
				TilesCopy.RemoveAt(Stream.RandRange(0,TilesCopy.Num()));	
			}
			Working = false;
			ConnectedTiles = TilesCopy;
		}
	}
	
		
		
		for (auto ConnectedTile : ConnectedTiles)
		{
			ExtentsX.Add(ConnectedTile.X);
			ExtentsY.Add(ConnectedTile.Y);	
			
		}
		int32 Index_X;
		int32 Index_Y;
		UKismetMathLibrary::MaxOfIntArray(ExtentsX,Index_X ,MaxExtents_X);
		UKismetMathLibrary::MaxOfIntArray(ExtentsY,Index_Y ,MaxExtents_Y);

	
	
	MaxExtents =FIntVector(MaxExtents_X,MaxExtents_Y,RootLocation.Z);
	FloorTiles.Append(ConnectedTiles);
	
}

void ADungeonGenerator::SpawnTiles()
{
	UWorld*	World = GetWorld();
	
for (auto FloorTile : FloorTiles)
{
	FVector SpawnLocation = UKismetMathLibrary::Conv_IntVectorToVector(FloorTile) * Scale;
	if(World&&FloorMesh)
	{
		FTransform SpawnTransform = FTransform(SpawnLocation);
		FloorMesh->AddInstance(SpawnTransform,true);
		
	}
	
}
	
}

bool ADungeonGenerator::Room_FindNextLocation(FIntVector &NewLocation)
{
	bool IsFloorTile;
	bool Searching =true;
	bool Valid = false;
	int32 TestIndex=0;
	TArray<int32> Directions;
	FIntVector CheckedLocation;
	
	

	
	for(int32 i = 0; i<8;i++)
	{
		Directions.Add(i);
	}
	while(Searching)
	{
		if(!Directions.IsEmpty())
		{
			TestIndex = Stream.RandRange(0,Directions.Num()-1);
			int32 PositionToCheck;
			if(Merging)
			{
				PositionToCheck = RoomSize_Max;
			}
			else
			{
				PositionToCheck = RoomSize_Max+1;
			}
			switch (TestIndex)
			{
			case 0:
				CheckedLocation = TestRelativeTileLocation(PreviousLocation,PositionToCheck,0,IsFloorTile);
				break;
			case 1:
				CheckedLocation = TestRelativeTileLocation(PreviousLocation,PositionToCheck,PositionToCheck,IsFloorTile);
				break;
			case 2:
				CheckedLocation = TestRelativeTileLocation(PreviousLocation,0,PositionToCheck,IsFloorTile);
				break;
			case 3:
				CheckedLocation = TestRelativeTileLocation(PreviousLocation,PositionToCheck*-1,PositionToCheck,IsFloorTile);
				break;
			case 4:
				CheckedLocation = TestRelativeTileLocation(PreviousLocation,PositionToCheck*-1,0,IsFloorTile);
				break;
			case 5:
				CheckedLocation = TestRelativeTileLocation(PreviousLocation,PositionToCheck*-1,PositionToCheck*-1,IsFloorTile);
				break;
			case 6:
				CheckedLocation = TestRelativeTileLocation(PreviousLocation,0,PositionToCheck*-1,IsFloorTile);
				break;
			case 7:
				CheckedLocation = TestRelativeTileLocation(PreviousLocation,PositionToCheck,PositionToCheck*-1,IsFloorTile);
				break;
			default:break;
			}
			if(IsFloorTile)
			{
				Directions.RemoveAt(TestIndex);
				Searching = true;
			}
			else
			{
				Searching = false;
				Valid = true;
				NewLocation = CheckedLocation;
			
			}
		}
		else
		{
			Searching = false;
			Valid = false;
		}
		
	}
	return Valid;
}

FIntVector ADungeonGenerator::TestRelativeTileLocation(FIntVector ReferenceLocation, int32 X,int32 Y ,bool &thisIsFloorTile )
{
	FIntVector Location =FIntVector(ReferenceLocation.X+X,ReferenceLocation.Y+Y,ReferenceLocation.Z);
	thisIsFloorTile = FloorTiles.Contains(Location);
	return Location;
	
	
	
	
	
}

// Called every frame
void ADungeonGenerator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

