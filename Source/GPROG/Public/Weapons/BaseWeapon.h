// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseWeapon.generated.h"

class USphereComponent;
class AGPROGCharacter;

UCLASS()
class GPROG_API ABaseWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABaseWeapon();
	
	

	/** Sound to play each time we fire */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Gameplay)
	USoundBase* FireSound;
	
	/** AnimMontage to play each time we fire */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	UAnimMontage* FireAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	float TimeBetweenShoots;


	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Meshes)
	TObjectPtr<USkeletalMeshComponent> GunMesh;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<USphereComponent> SphereComponent;
	
	/** Attaches the actor to a FirstPersonCharacter */
	UFUNCTION(BlueprintCallable, Category="Weapon")
	void AttachWeapon(AGPROGCharacter* TargetCharacter);

	

protected:

	
	
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	void OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	FTimerHandle Timerhandle_Rifle;

	

private:
	/** The Character holding this weapon*/
	UPROPERTY(EditAnywhere)
	AGPROGCharacter* Character;

	

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
