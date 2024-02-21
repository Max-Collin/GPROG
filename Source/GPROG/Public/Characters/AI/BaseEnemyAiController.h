// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionTypes.h"
#include "BaseEnemyAiController.generated.h"


class UAISenseConfig_Sight;
class UAISenseConfig_Hearing;
struct FAIStimulus;

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

	TObjectPtr<class UAIPerceptionComponent> UaiPerceptionComponent;
	
	TObjectPtr<class UAISenseConfig_Sight> Sight;

public:

	
void GetRandomLocation();

	ABaseEnemyAiController();

protected:
	virtual void BeginPlay() override;
	
private:
	UFUNCTION()
	void OnTargetPerceptionUpdate(AActor* SeenActor,FAIStimulus Stimulus);
	
public:
	FORCEINLINE UBlackboardComponent* GetBlackboardComponent() const {return  BlackboardComponent;}
};
