// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/BaseItem.h"
#include "EndDoor.generated.h"

/**
 * 
 */
UCLASS()
class GPROG_API AEndDoor : public ABaseItem
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	TObjectPtr<UStaticMeshComponent>  SphereMeshComponent;

public:
	AEndDoor();
	
	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly)
	FName LevelToLoad = TEXT("TestMap");

	UPROPERTY(EditAnywhere)
	bool isShopDoor=false;
private:
	UFUNCTION()
	void OpenDoor();

	bool bCanNextLevel=false;

	UPROPERTY(EditDefaultsOnly)
	UMaterialInstance* Red;
	
	UPROPERTY(EditDefaultsOnly)
	UMaterialInstance* Green;
	
public:
	
};
