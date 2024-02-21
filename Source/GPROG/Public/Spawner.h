// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Spawner.generated.h"


class ADungeonGenerator;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GPROG_API USpawner : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USpawner();
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void NewLevel();
	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
	FVector offset;
	
	UPROPERTY(EditAnywhere)
	int32 Quantity=0;
	
	UPROPERTY(EditAnywhere)
	bool RoomOnly = true;
	
	UPROPERTY(EditAnywhere)
	TMap<TSubclassOf<AActor>,float> SpawnList;


	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> EndDoor;
	
	//UPROPERTY(EditDefaultsOnly)
	//TObjectPtr<>
	

private:
	void Spawn();
	void SpawnItems();
	void SpawnEndDoor();

	float Scale;
	FRandomStream Stream;

	UPROPERTY(VisibleAnywhere)
	TArray<FIntVector> SpawnLocationTiles;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<ADungeonGenerator> DungeonGenerator;
	
	

public:	
	
		
};
