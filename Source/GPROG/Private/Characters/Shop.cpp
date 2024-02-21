// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Shop.h"

#include "Characters/Player/GPROGCharacter.h"

AShop::AShop()
{
	
}

void AShop::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

	if(OtherActor->ActorHasTag("Player"))
	{
		Cast<AGPROGCharacter>(OtherActor)->SetOverlappingItem(this);
	}
}

void AShop::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if(OtherActor->ActorHasTag("Player"))
	{
		Cast<AGPROGCharacter>(OtherActor)->SetOverlappingItem(nullptr);
	}
	
}

void AShop::BeginPlay()
{
	Super::BeginPlay();


}
