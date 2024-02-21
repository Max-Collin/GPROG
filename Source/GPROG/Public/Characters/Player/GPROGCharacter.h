// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/BaseCharacter.h"
#include "Interfaces/PickUp_Interface.h"
#include "GPROGCharacter.generated.h"

class ABaseItem;
class ABaseWeapon;
class UInputComponent;
class USkeletalMeshComponent;
class UCameraComponent;
class UInputAction;
class UInputMappingContext;
struct FInputActionValue;
class USpotLightComponent;

/**
 * 
 */
UCLASS()
class GPROG_API AGPROGCharacter : public ABaseCharacter , public IPickUp_Interface
{
	GENERATED_BODY()


	
	

public:
	AGPROGCharacter();

	void AttachWeapon(ABaseWeapon* NewWeapon);
	virtual void SetOverlappingItem(class ABaseItem* Item) override;

	virtual void AddTreasure() override;
	virtual void Hit() override;

	/** Make the weapon Fire a Projectile */
	UFUNCTION(BlueprintCallable, Category="Weapon")
	void Fire();

	void StartFire();
	void EndFire();
	
	UFUNCTION(BlueprintCallable)
	 float GetMaxHealth( ) ;
	UFUNCTION(BlueprintCallable)
	float GetHealth( ) ;
protected:
	UFUNCTION()
	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	virtual void BeginPlay();

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
	
	void Move(const FInputActionValue& Value);

	
	void Look(const FInputActionValue& Value);
	
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	FTimerHandle Timerhandle_Rifle;
	
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UCameraComponent> CameraComponent;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<USceneComponent> SceneComponent;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<USkeletalMeshComponent> Mesh1P;
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ABaseWeapon> DefaultWeapon;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<ABaseWeapon> EquippedWeapon;

	/** Sound to play each time we fire */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Gameplay)
	USoundBase* FireSound;
	
	/** AnimMontage to play each time we fire */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	UAnimMontage* FireAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	float TimeBetweenShoots;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	UParticleSystem* ImpactParticles;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* JumpAction;

	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* LookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* FireAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* FlashlightAction;

	UPROPERTY(EditAnywhere)
	TObjectPtr<USpotLightComponent> FlashLightComponent;
	

private:

	void ToggleFlashlight();
	bool HasWeapon;
	bool bFlashlight =true;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<ABaseItem> OverlappingItem;
	
public:
	FORCEINLINE USkeletalMeshComponent* GetMesh1p() const {return  Mesh1P;}
	FORCEINLINE bool GetHasWeapon() const {return  HasWeapon;}
	FORCEINLINE void SetHasWeapon(bool bNewWeapon) {HasWeapon = bNewWeapon;}
	FORCEINLINE UCameraComponent* GetCameraComponent() const {return CameraComponent;}
	FORCEINLINE void SetMaxHealth(float NewHealth){MaxHealth = NewHealth; }
	FORCEINLINE void RegenHealth( ){Health = MaxHealth; }
	
	
	
};
