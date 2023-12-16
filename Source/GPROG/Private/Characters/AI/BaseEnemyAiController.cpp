// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/AI/BaseEnemyAiController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AI/NavigationSystemBase.h"
#include "NavigationSystem.h"

void ABaseEnemyAiController::GetRandomLocation()
{
	if(BlackboardComponent)
	{
	
	
	
	}
}

ABaseEnemyAiController::ABaseEnemyAiController()
{
	BlackboardComponent = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackboardComponent"));
}

void ABaseEnemyAiController::BeginPlay()
{
	Super::BeginPlay();


	if(BehaviorTree)
	{
		RunBehaviorTree(BehaviorTree);
	}
	if(BlackboardComponent && BlackboardData)
	{
		InitializeBlackboard(*BlackboardComponent,*BlackboardData);
	}
}
