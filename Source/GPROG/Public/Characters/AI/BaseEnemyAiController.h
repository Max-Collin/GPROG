// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "BaseEnemyAiController.generated.h"

/**
 * 
 */
UCLASS()
class GPROG_API ABaseEnemyAiController : public AAIController
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UBehaviorTree> BehaviorTree;
	
	TObjectPtr<UBlackboardComponent> BlackboardComponent;
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UBlackboardData> BlackboardData;

public:
	
void GetRandomLocation();

	ABaseEnemyAiController();

protected:
	virtual void BeginPlay() override;
	
private:
	
public:
	FORCEINLINE UBlackboardComponent* GetBlackboardComponent() const {return  BlackboardComponent;}
};
