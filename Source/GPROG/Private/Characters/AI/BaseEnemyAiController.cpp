// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/AI/BaseEnemyAiController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AI/NavigationSystemBase.h"
#include "NavigationSystem.h"

#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig.h"
#include "Perception/AISenseConfig_Sight.h"

void ABaseEnemyAiController::GetRandomLocation()
{
	if(BlackboardComponent)
	{
	
	
	
	}
}

ABaseEnemyAiController::ABaseEnemyAiController()
{
	BlackboardComponent = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackboardComponent"));

	SetPerceptionComponent(*CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AI Perception Component")));
	
	

	Sight=CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Config"));
	
	Sight->SightRadius=2000.f;
	Sight->LoseSightRadius=1500.f;
	Sight->PeripheralVisionAngleDegrees = 60.f;
	Sight->DetectionByAffiliation.bDetectEnemies=true;
	Sight->DetectionByAffiliation.bDetectNeutrals=true;
	Sight->DetectionByAffiliation.bDetectFriendlies=true;

	AAIController::GetPerceptionComponent()->ConfigureSense(*Sight);
	
	AAIController::GetPerceptionComponent()->SetDominantSense(*Sight->GetSenseImplementation());
	AAIController::GetPerceptionComponent()->OnTargetPerceptionUpdated.AddDynamic(this,&ABaseEnemyAiController::OnTargetPerceptionUpdate);
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

void ABaseEnemyAiController::OnTargetPerceptionUpdate(AActor* SeenActor, FAIStimulus Stimulus)
{
	if(SeenActor)
	{
		if(Stimulus.Type == UAISense::GetSenseID<UAISense_Sight>())
		{
			
			if(BlackboardComponent)
			{
				BlackboardComponent->SetValueAsObject("Player",SeenActor);
			
			}
        		
		}
	}
}
