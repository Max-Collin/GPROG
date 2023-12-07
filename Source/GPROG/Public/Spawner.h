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

	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly)
	FVector offset;
	
	UPROPERTY(EditDefaultsOnly)
	int32 Quantity;
	
	UPROPERTY(EditDefaultsOnly)
	bool RoomOnly = true;
	
	UPROPERTY(EditDefaultsOnly)
	TMap<TSubclassOf<AActor>,float> SpawnList;

	//UPROPERTY(EditDefaultsOnly)
	//TObjectPtr<>
	

private:
	void Spawn();
	void SpawnItems();

	float Scale;
	FRandomStream Stream;

	UPROPERTY(VisibleAnywhere)
	TArray<FIntVector> SpawnLocationTiles;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<ADungeonGenerator> DungeonGenerator;
	

public:	
	
		
};
