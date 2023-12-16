// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/BaseCharacter.h"
#include "BaseEnemy.generated.h"

/**
 * 
 */
UCLASS()
class GPROG_API ABaseEnemy : public ABaseCharacter
{
	GENERATED_BODY()


public:
	virtual void Hit() override;
	
};
