// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/enemies/BaseEnemy.h"

#include "Singletons/GameSingleton.h"

void ABaseEnemy::Hit()
{
	Super::Hit();

	Health-=20;
	//UE_LOG(LogTemp,Warning,TEXT("Enemy Health = %f"),Health)
	if(Health<=0)
	{
		
		Destroy();
		UGameSingleton* GameSingleton = UGameSingleton::GetInstance();
		if(GameSingleton)
		{
			GameSingleton->KilledEnemy();
		
		}
	}
}
