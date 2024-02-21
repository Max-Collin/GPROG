// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/BaseCharacter.h"
#include "Items/BaseItem.h"
#include "Shop.generated.h"

/**
 * 
 */
UCLASS()
class GPROG_API AShop : public ABaseItem
{
	GENERATED_BODY()

	
	
public:
	 AShop();
	
	 void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override ;
	
	virtual  void OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;

protected:
	virtual void BeginPlay() override;
};
