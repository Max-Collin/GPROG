// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_Shoot.generated.h"

class AProjectile;
/**
 * 
 */
UCLASS()
class GPROG_API UBTTask_Shoot : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
protected:
	UPROPERTY(EditAnywhere)
	TSubclassOf<AProjectile> Projectile;

	UPROPERTY(EditAnywhere)
	FBlackboardKeySelector player;
private:
public:
	
};
