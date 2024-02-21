// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/AI/Tasks/BTTask_Shoot.h"

#include "Projectile.h"
#include "Characters/AI/BaseEnemyAiController.h"
#include "Characters/enemies/BaseEnemy.h"

EBTNodeResult::Type UBTTask_Shoot::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	
	ABaseEnemyAiController* EnemyAIController = Cast<ABaseEnemyAiController>( OwnerComp.GetOwner());
	ABaseEnemy* Enemy = Cast<ABaseEnemy>( EnemyAIController->GetPawn());
	if(EnemyAIController&&Enemy)
	{
	 const FVector  SpawnLocation = Enemy->GetActorLocation();
		AProjectile* CurrentProjectile = GetWorld()->SpawnActor<AProjectile>(Projectile,SpawnLocation,Enemy->GetActorRotation());
		
	}

	return EBTNodeResult::Succeeded;
	
}
