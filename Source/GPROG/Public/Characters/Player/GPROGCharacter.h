// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/BaseCharacter.h"
#include "GPROGCharacter.generated.h"

class ABaseWeapon;
class UInputComponent;
class USkeletalMeshComponent;
class UCameraComponent;
class UInputAction;
class UInputMappingContext;
struct FInputActionValue;

/**
 * 
 */
UCLASS()
class GPROG_API AGPROGCharacter : public ABaseCharacter
{
	GENERATED_BODY()


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
	

public:
	AGPROGCharacter();

	void AttachWeapon(ABaseWeapon* NewWeapon);

	/** Make the weapon Fire a Projectile */
	UFUNCTION(BlueprintCallable, Category="Weapon")
	void Fire();

	void StartFire();
	void EndFire();
protected:

	virtual void BeginPlay();

	
	
	
	void Move(const FInputActionValue& Value);

	
	void Look(const FInputActionValue& Value);
	
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UCameraComponent> CameraComponent;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<USceneComponent> SceneComponent;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<USkeletalMeshComponent> Mesh1P;
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ABaseWeapon> DefaultWeapon;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<ABaseWeapon> EquipedWeapon;
private:

	bool HasWeapon;
public:
	FORCEINLINE USkeletalMeshComponent* GetMesh1p() const {return  Mesh1P;}
	FORCEINLINE bool GetHasWeapon() const {return  HasWeapon;}
	FORCEINLINE void SetHasWeapon(bool bNewWeapon) {HasWeapon = bNewWeapon;}
	FORCEINLINE UCameraComponent* GetCameraComponet() const {return CameraComponent;}
	
};
