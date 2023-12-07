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
	Root = CreateDefaultSubobject<USceneComponent>("Root");
	RootComponent = Root;

	
	FloorMesh = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("Floor Mesh"));
	FloorMesh->SetupAttachment(GetRootComponent());
	
	WallMesh = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("Wall Mesh"));
	WallMesh->SetupAttachment(GetRootComponent());
	
	
}

void ADungeonGenerator::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	FloorMesh->ClearInstances();
	WallMesh->ClearInstances();
	FloorTiles.Empty();
	CorridorTiles.Empty();
	Rooms.Empty();
	PreviousLocation = FIntVector(0,0,0);
	Seed = FMath::Rand() % 999999999;
	if(NewSeed)
	{
		NewSeed = false;
		Stream.Initialize(Seed);
	}
	
	
	Stream.Initialize(Seed);
	GenerateMap();
}

// Called when the game starts or when spawned
void ADungeonGenerator::BeginPlay()
{
	Super::BeginPlay();
	
	
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

				CorridorTiles.Append(Corridors_MapCorridors(PreviousLocation,NewLocation));
				
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
	bool IsFloorTile;
	UWorld*	World = GetWorld();


	FloorTiles.Append(CorridorTiles);
	
for (auto FloorTile : FloorTiles)
{
	FVector SpawnLocation = UKismetMathLibrary::Conv_IntVectorToVector(FloorTile) * Scale;
	if(World&&FloorMesh)
	{
		FTransform SpawnTransform = FTransform(SpawnLocation);
		FloorMesh->AddInstance(SpawnTransform,true);
		
	}
	


	for(int32 i =0 ; i<4;i++)
	{
		FRotator Rotation;
		switch (i)
		{
		case 0:
			TestRelativeTileLocation(FloorTile,1,0,IsFloorTile);
			if(!IsFloorTile)
			{
				FVector SpawnWallLocation =FVector(SpawnLocation.X+400,SpawnLocation.Y,SpawnLocation.Z);
				Rotation=FRotator(0.f,90.f,0.f);
				
				FTransform SpawnTransform = FTransform(Rotation,SpawnWallLocation);
			WallMesh->AddInstance(SpawnTransform);
			}
			break;
		case 1:
			TestRelativeTileLocation(FloorTile,0,1,IsFloorTile);
			if(!IsFloorTile)
			{
				FVector SpawnWallLocation =FVector(SpawnLocation.X,SpawnLocation.Y+400,SpawnLocation.Z);
				Rotation=FRotator(0.f,0.f,0.f);
				FTransform SpawnTransform = FTransform(Rotation,SpawnWallLocation);
				WallMesh->AddInstance(SpawnTransform);
			}
			break;
		case 2:
			TestRelativeTileLocation(FloorTile,-1,0,IsFloorTile);
			if(!IsFloorTile)
			{
				FVector SpawnWallLocation =FVector(SpawnLocation.X,SpawnLocation.Y+400,SpawnLocation.Z);
				Rotation=FRotator(0.f,-90.f,0.f);
				FTransform SpawnTransform = FTransform(Rotation,SpawnWallLocation);
				WallMesh->AddInstance(SpawnTransform);
			}
			break;
		case 3:
			TestRelativeTileLocation(FloorTile,0,-1,IsFloorTile);
			if(!IsFloorTile)
			{
				FVector SpawnWallLocation =FVector(SpawnLocation.X+400,SpawnLocation.Y,SpawnLocation.Z);
				Rotation=FRotator(0.f,180.f,0.f);
				FTransform SpawnTransform = FTransform(Rotation,SpawnWallLocation);
				WallMesh->AddInstance(SpawnTransform);
			}
			break;
			default:break;
		}
		
	
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
	
	

	
	for(int32 i = 0; i<4;i++)
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
			/*case 0:
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
				break;*/

				case 0:
					CheckedLocation = TestRelativeTileLocation(PreviousLocation,PositionToCheck,0,IsFloorTile);
				break;
			case 1:
					CheckedLocation = TestRelativeTileLocation(PreviousLocation,0,PositionToCheck,IsFloorTile);
					break;
			case 2:
				CheckedLocation = TestRelativeTileLocation(PreviousLocation,PositionToCheck*-1,0,IsFloorTile);
				break;
			case 3:
					CheckedLocation = TestRelativeTileLocation(PreviousLocation,0,PositionToCheck*-1,IsFloorTile);
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

TArray<FIntVector> ADungeonGenerator::Corridors_MapCorridors(FIntVector RoomA, FIntVector RoomB)
{
	UE_LOG(LogTemp,Warning,TEXT("Corridors_MapCorridors"))
	FIntVector RoomAExtents = Rooms.FindRef(RoomA);
	FIntVector RoomBExtents = Rooms.FindRef(RoomB);

	int32 ParallelXMaxValue = UKismetMathLibrary::Max(RoomA.X,RoomB.X);
	int32 ParallelXMinValue = UKismetMathLibrary::Min(RoomAExtents.X,RoomBExtents.X);
	
	int32 ParallelYMaxValue = UKismetMathLibrary::Max(RoomA.Y,RoomB.Y);
	int32 ParallelYMinValue = UKismetMathLibrary::Min(RoomAExtents.Y,RoomBExtents.Y);

	int32 OutY;
	int32 OutX;
	FIntVector PointA;
	FIntVector PointB;

	TArray<FIntVector> Tiles;
	//UE_LOG(LogTemp,Warning,TEXT("ParallelMaxValue %d"),ParallelXMaxValue);
	//UE_LOG(LogTemp,Warning,TEXT("ParallelXMinValue %d"),ParallelXMinValue);
	//are rooms parallel on X with overlapping sides
	if(ParallelXMaxValue<=ParallelXMinValue)
	{
		UE_LOG(LogTemp,Warning,TEXT("are rooms parallel on X with overlapping sides"))
		if(RoomB.Y>RoomA.Y)//is room B to the right of room A
		{
			if(RoomB.Y-RoomAExtents.Y>1)//checks if rooms are not merged
			{
				//makes corridor from A to B on Y axis
				OutX =	Stream.RandRange(ParallelXMaxValue,ParallelXMinValue);
				PointA =FIntVector(OutX,RoomAExtents.Y,RoomA.Z);
				PointB = FIntVector(OutX,RoomB.Y,RoomB.Z);

				Tiles = Corridors_MakeY(PointA,PointB);
			}
			
		}
		else 
		{
			if(RoomA.Y-RoomBExtents.Y>1)//checks if rooms are not merged
				{
				//makes corridor from B to A on Y axis
				OutX =	Stream.RandRange(ParallelXMaxValue,ParallelXMinValue);
				PointA =FIntVector(OutX,RoomA.Y,RoomA.Z);
				PointB = FIntVector(OutX,RoomBExtents.Y,RoomB.Z);
				Tiles = Corridors_MakeY(PointB,PointA);
				}
		}
	}
	else  //are rooms parallel on Y with overlapping sides
	{

		UE_LOG(LogTemp,Warning,TEXT("are rooms parallel on Y with overlapping sides"))
		if(RoomB.X>RoomA.X)//is room B forward of room A
			{
			if(RoomB.X-RoomAExtents.X>1)//checks if rooms are not merged
				{
				//makes corridor from A to B on X axis
				OutY =	Stream.RandRange( ParallelYMaxValue, ParallelYMinValue);
				PointA =FIntVector(RoomAExtents.X,OutY,RoomA.Z);
				PointB = FIntVector(RoomB.X,OutY,RoomB.Z);
				Tiles = Corridors_MakeX(PointA,PointB);
				}
				else{UE_LOG(LogTemp,Warning,TEXT("rooms are merged"))}
			}
		else
		{
			if(RoomA.X-RoomBExtents.X>1)//checks if rooms are not merged
				{
				//makes corridor from B to A on X axis
				OutY =	Stream.RandRange( ParallelYMaxValue, ParallelYMinValue);
				PointA =FIntVector(RoomA.X,OutY,RoomA.Z);
				PointB = FIntVector(RoomBExtents.X,OutY,RoomB.Z);
				Tiles = Corridors_MakeX(PointB,PointA);
				}
			else{UE_LOG(LogTemp,Warning,TEXT("rooms are merged"))}
		}
	}
	return Tiles;
}

TArray<FIntVector> ADungeonGenerator::Corridors_MakeY(FIntVector From, FIntVector To)
{
	UE_LOG(LogTemp,Warning,TEXT("Make Y"))
	TArray<FIntVector> Tiles;
	int32 MaxIterations = FMath::Abs(From.Y-To.Y );
	for (int32 i=0;i<MaxIterations;i++)
	{
		FIntVector tile = FIntVector(From.X,From.Y+i,From.Z);
		Tiles.Add(tile);
	}
	return Tiles;
}

TArray<FIntVector> ADungeonGenerator::Corridors_MakeX(FIntVector From, FIntVector To)
{
	UE_LOG(LogTemp,Warning,TEXT("Make X"))
	TArray<FIntVector> Tiles;
	int32 MaxIterations = FMath::Abs(From.X-To.X );
	for (int32 i=0;i<MaxIterations;i++)
	{
		FIntVector tile = FIntVector(From.X+i,From.Y,From.Z);
		Tiles.Add(tile);
	}
	return Tiles;
}

// Called every frame
void ADungeonGenerator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

