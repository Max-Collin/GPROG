// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/SpeedPickup.h"

#include "Characters/Player/GPROGCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

void ASpeedPickup::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                   UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AGPROGCharacter* Player = Cast<AGPROGCharacter>(OtherActor);
	if(Player)
	{
		Player->GetCharacterMovement()->MaxWalkSpeed = 1000;
		
	}
	Destroy();
}
