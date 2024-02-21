// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/BaseItem.h"

#include "Components/SphereComponent.h"
#include "Interfaces/PickUp_Interface.h"
#include "Singletons/GameManager.h"
#include "Singletons/GameSingleton.h"

// Sets default values
ABaseItem::ABaseItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere Component"));
	SetRootComponent(SphereComponent);

}

// Called when the game starts or when spawned
void ABaseItem::BeginPlay()
{
	Super::BeginPlay();
	SphereComponent->OnComponentBeginOverlap.AddDynamic(this,&ABaseItem::OnSphereOverlap);
	SphereComponent->OnComponentEndOverlap.AddDynamic(this,&ABaseItem::OnSphereEndOverlap);
	
}

// Called every frame
void ABaseItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
void ABaseItem::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	
	IPickUp_Interface* PickUpInterface = Cast<IPickUp_Interface>(OtherActor);
	
	if(PickUpInterface)
	{
		UGameSingleton* GameSingleton = UGameSingleton::GetInstance();

		if(GameSingleton)
		{

			GameSingleton->AddTreasure(this);
			PickUpInterface->AddTreasure();
			Destroy();
			
		}
		
	}
}

void ABaseItem::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	IPickUp_Interface* PickUpInterface = Cast<IPickUp_Interface>(OtherActor);
	
	if(PickUpInterface)
	{
		PickUpInterface->SetOverlappingItem(nullptr);
	}
}
