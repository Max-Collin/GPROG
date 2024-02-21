// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Player/GPROGCharacter.h"

#include "Camera/CameraComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Characters/enemies/BaseEnemy.h"
#include "Components/SpotLightComponent.h"
#include "Interfaces/PickUp_Interface.h"
#include "Items/BaseItem.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Singletons/GameSingleton.h"

#include "Weapons/BaseWeapon.h"


AGPROGCharacter::AGPROGCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	//SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root Component"));
	//SetRootComponent(SceneComponent);
	
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComponent->SetupAttachment(GetRootComponent());// attaches camera to root component
	CameraComponent->SetRelativeLocation(FVector(-10.f, 0.f, 60.f)); // Position of the camera
    CameraComponent->bUsePawnControlRotation = true; // camera rotates with the player

	GetMesh()->SetOwnerNoSee(true);

	
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetupAttachment(CameraComponent);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	Mesh1P->SetRelativeLocation(FVector(-30.f, 0.f, -150.f));

	FlashLightComponent  =CreateDefaultSubobject<USpotLightComponent>(TEXT("FlashLight"));
	FlashLightComponent->SetupAttachment(CameraComponent);
	Tags.Add("Player");
}

void AGPROGCharacter::AttachWeapon(ABaseWeapon* NewWeapon)
{
	

	
}

void AGPROGCharacter::SetOverlappingItem(ABaseItem* Item)
{
	IPickUp_Interface::SetOverlappingItem(Item);

	OverlappingItem = Item;
}

void AGPROGCharacter::AddTreasure()
{
	IPickUp_Interface::AddTreasure();
	
}

void AGPROGCharacter::Hit()
{
	Super::Hit();

	Health= Health - 20;
	if(Health<=0)
	{
		UE_LOG(LogTemp,Warning,TEXT("Player has perished"))
	}
}

void AGPROGCharacter::Fire()
{

FHitResult HitResult;

	const float WeaponRange = 20000.f;
	const FVector StartTrace =GetCameraComponent()->GetComponentLocation();
	const FVector EndTrace =(UKismetMathLibrary::GetForwardVector(GetCameraComponent()->GetComponentRotation())*WeaponRange)+StartTrace;

	
	//FCollisionQueryParams QueryParams = FCollisionQueryParams(SCENE_QUERY_STAT(WeaponTrace,false,this));
	
	bool bHitSomething = GetWorld()->LineTraceSingleByChannel(HitResult,StartTrace,EndTrace,ECC_Visibility);
	if(bHitSomething)
	{
		//UE_LOG(LogTemp,Warning,TEXT("hit %s"),*HitResult.GetActor()->GetName());
		if(ImpactParticles)
		{
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(),ImpactParticles,FTransform(HitResult.ImpactNormal.Rotation(),HitResult.ImpactPoint));
		}
		if(IHitableActor* HitableActor = Cast<IHitableActor>(HitResult.GetActor()))
		{
			HitableActor->Hit();
		}
		
	}
	// Try and play the sound if specified
	if (FireSound )
	{
		UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
	}
}

void AGPROGCharacter::StartFire()
{
	Fire();
	GetWorldTimerManager().SetTimer(Timerhandle_Rifle,this,&AGPROGCharacter::Fire,TimeBetweenShoots,true);
}

void AGPROGCharacter::EndFire()
{
	GetWorldTimerManager().ClearTimer(Timerhandle_Rifle);
}

float AGPROGCharacter::GetMaxHealth()
{
	return MaxHealth;
}

float AGPROGCharacter::GetHealth()
{
	return Health;
}

void AGPROGCharacter::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                      UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	  


	
}


void AGPROGCharacter::BeginPlay()
{
	Super::BeginPlay();
	if(DefaultWeapon)
	{
		EquippedWeapon = GetWorld()->SpawnActor<ABaseWeapon>(DefaultWeapon);
		FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, true);
		EquippedWeapon->AttachToComponent(Mesh1P, AttachmentRules, FName(TEXT("GripPoint")));
		
	}
	

	// Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}

	if(UGameSingleton* GameSingleton = UGameSingleton::GetInstance())
	{
		GameSingleton->ResetScore();
	}
}

void AGPROGCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	if(UGameSingleton* GameSingleton = UGameSingleton::GetInstance())
	{
		GameSingleton->StoreScore();
	}
}

void AGPROGCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add movement 
		AddMovementInput(GetActorForwardVector(), MovementVector.Y);
		AddMovementInput(GetActorRightVector(), MovementVector.X);
	}
}

void AGPROGCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void AGPROGCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AGPROGCharacter::Move);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AGPROGCharacter::Look);
		//fire
		EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Started, this, &AGPROGCharacter::StartFire);
		EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Completed, this, &AGPROGCharacter::EndFire);
		//flashlight
		EnhancedInputComponent->BindAction(FlashlightAction, ETriggerEvent::Started, this, &AGPROGCharacter::ToggleFlashlight);
	}
}

void AGPROGCharacter::ToggleFlashlight()
{
	UE_LOG(LogTemp,Warning,TEXT("Flashlight"))
	FlashLightComponent->ToggleVisibility();
	
	
	
}

