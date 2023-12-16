// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Player/GPROGCharacter.h"

#include "Camera/CameraComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Weapons/BaseWeapon.h"


AGPROGCharacter::AGPROGCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	//SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root Component"));
	//SetRootComponent(SceneComponent);
	
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComponent->SetupAttachment(GetRootComponent());
	CameraComponent->SetRelativeLocation(FVector(-10.f, 0.f, 60.f)); // Position the camera
    CameraComponent->bUsePawnControlRotation = true;

	GetMesh()->SetOwnerNoSee(true);

	
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetupAttachment(CameraComponent);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	Mesh1P->SetRelativeLocation(FVector(-30.f, 0.f, -150.f));

	
}

void AGPROGCharacter::AttachWeapon(ABaseWeapon* NewWeapon)
{
	

	
}

void AGPROGCharacter::Hit()
{
	Super::Hit();
}

void AGPROGCharacter::Fire()
{

FHitResult HitResult;

	const float WeaponRange = 20000.f;
	const FVector StartTrace =GetCameraComponet()->GetComponentLocation();
	const FVector EndTrace =(UKismetMathLibrary::GetForwardVector(GetCameraComponet()->GetComponentRotation())*WeaponRange)+StartTrace;

	
	//FCollisionQueryParams QueryParams = FCollisionQueryParams(SCENE_QUERY_STAT(WeaponTrace,false,this));
	
	bool bHitSomething = GetWorld()->LineTraceSingleByChannel(HitResult,StartTrace,EndTrace,ECC_Visibility);
	if(bHitSomething)
	{
		if(ImpactParticles)
		{
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(),ImpactParticles,FTransform(HitResult.ImpactNormal.Rotation(),HitResult.ImpactPoint));
		}
		UE_LOG(LogTemp,Warning,TEXT("hit %s"),*HitResult.GetActor()->GetName());
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


void AGPROGCharacter::BeginPlay()
{
	Super::BeginPlay();
	if(DefaultWeapon)
	{
		EquipedWeapon = GetWorld()->SpawnActor<ABaseWeapon>(DefaultWeapon);
		FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, true);
		EquipedWeapon->AttachToComponent(Mesh1P, AttachmentRules, FName(TEXT("GripPoint")));
		
	}
	

	// Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
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

		EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Started, this, &AGPROGCharacter::StartFire);
		EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Completed, this, &AGPROGCharacter::EndFire);
	}
}

