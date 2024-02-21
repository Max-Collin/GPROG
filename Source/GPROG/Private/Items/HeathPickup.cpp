// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/HeathPickup.h"

#include "Characters/Player/GPROGCharacter.h"

void AHeathPickup::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                   UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	
	AGPROGCharacter* Player = Cast<AGPROGCharacter>(OtherActor);
	if(Player)
	{
		Player->SetMaxHealth(200);
		Player->RegenHealth();
	}
	Destroy();
}

void AHeathPickup::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	


	
}
