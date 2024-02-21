// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/AI/Tasks/T_GetRandomLocation.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Characters/AI/BaseEnemyAiController.h"
#include "Characters/enemies/BaseEnemy.h"

EBTNodeResult::Type UT_GetRandomLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{

	ABaseEnemyAiController* EnemyAIController = Cast<ABaseEnemyAiController>( OwnerComp.GetOwner());
	ABaseEnemy* Enemy = Cast<ABaseEnemy>( EnemyAIController->GetPawn());
	if(EnemyAIController&&Enemy)
	{

		
		 
		UNavigationSystemV1* NavigationSystem  = UNavigationSystemV1::GetCurrent(GetWorld());		
		
		FNavLocation NavLocation;
		
		if(NavigationSystem->GetRandomReachablePointInRadius(Enemy->GetActorLocation(),2000.f,NavLocation))
		{
			EnemyAIController->GetBlackboardComponent()->SetValueAsVector("RandomLocation",NavLocation.Location);
			
		}


		
		
		FinishLatentTask(OwnerComp,EBTNodeResult::Succeeded);
		return EBTNodeResult::Succeeded;
	}
	
	
		return EBTNodeResult::Failed;
	


	
	
}
