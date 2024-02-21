// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/EndDoor.h"

#include "DungeonGenerator.h"
#include "Spawner.h"

#include "Singletons/GameSingleton.h"

AEndDoor::AEndDoor()
{
	SphereMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh Component"));
	SphereMeshComponent->SetupAttachment(RootComponent);
}

void AEndDoor::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(isShopDoor&&OtherActor->ActorHasTag("Player"))
	{
		UGameSingleton* GameSingleton = UGameSingleton::GetInstance();
		if(GameSingleton)
		{
			ADungeonGenerator* dungeon = GameSingleton->DungeonGenerator;
			Cast<USpawner>(dungeon->SpawnerComponent)->NewLevel();
		}
		
		OtherActor->SetActorLocation(FVector(200,200,200));
		
	}
	if(bCanNextLevel&&OtherActor->ActorHasTag("Player"))
	{
	
		
	
			UGameSingleton* GameSingleton = UGameSingleton::GetInstance();
			if(GameSingleton)
			{
				ADungeonGenerator* dungeon = GameSingleton->DungeonGenerator;
				dungeon->NewMap();
				OtherActor->SetActorLocation(FVector(6023.f,-34720.f,12245.f));
				GameSingleton->ResetGameValues();
				Destroy();
			}
		
		
		
	}
	
}

void AEndDoor::BeginPlay()
{
	Super::BeginPlay();
	if (isShopDoor)
	{
		SphereMeshComponent->SetMaterial(0,Green);
	}
	else
	{
		UGameSingleton* GameSingleton = UGameSingleton::GetInstance();
		if(GameSingleton)
		{
			GameSingleton->OnAllTreasure.AddDynamic(this,&AEndDoor::OpenDoor);
			SphereMeshComponent->SetMaterial(0,Red);
		}

	
	}
		
	

	
}

void AEndDoor::OpenDoor()
{
	SphereMeshComponent->SetMaterial(0,Green);
	bCanNextLevel = true;

	
}
