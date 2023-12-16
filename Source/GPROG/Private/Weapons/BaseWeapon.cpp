// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/BaseWeapon.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Characters/Player/GPROGCharacter.h"
#include "Components/SphereComponent.h"


// Sets default values
ABaseWeapon::ABaseWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("PickUp Sphere"));
	
	
	SetRootComponent(SphereComponent);
	GunMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Gun Mesh"));
	GunMesh->SetupAttachment(GetRootComponent());
}

void ABaseWeapon::AttachWeapon(AGPROGCharacter* TargetCharacter)
{
	Character = TargetCharacter;

	// Check that the character is valid, and has no rifle yet
	if (Character == nullptr || Character->GetHasWeapon())
	{
		return;
	}

	
	
}

/*
void ABaseWeapon::Fire()
{
	
	FHitResult HitResult;

	const float WeaponRange = 20000.f;
	const FVector StartTrace =Character->GetCameraComponet()->GetComponentLocation();
	const FVector EndTrace =(Character->GetActorForwardVector()*WeaponRange)+StartTrace;

	//FCollisionQueryParams QueryParams = FCollisionQueryParams(SCENE_QUERY_STAT(WeaponTrace,false,this));
	
	bool bHitSomething = GetWorld()->LineTraceSingleByChannel(HitResult,StartTrace,EndTrace,ECC_Visibility);
	if(bHitSomething)
	{
		UE_LOG(LogTemp,Warning,TEXT("hit"));
	}
	// Try and play the sound if specified
	if (FireSound && Character)
	{
		UGameplayStatics::PlaySoundAtLocation(this, FireSound, Character->GetActorLocation());
	}
	
	// Try and play a firing animation if specified
	if (FireAnimation && Character)
	{
		// Get the animation object for the arms mesh
		UAnimInstance* AnimInstance = Character->GetMesh1p()->GetAnimInstance() ;
		if (AnimInstance != nullptr)
		{
			AnimInstance->Montage_Play(FireAnimation, 1.f);
		}
	}
}

void ABaseWeapon::StartFire()
{
	Fire();
	GetWorldTimerManager().SetTimer(Timerhandle_Rifle,this,&ABaseWeapon::Fire,TimeBetweenShoots,true);
}

void ABaseWeapon::EndFire()
{
	GetWorldTimerManager().ClearTimer(Timerhandle_Rifle);
}
*/

// Called when the game starts or when spawned
void ABaseWeapon::BeginPlay()
{
	Super::BeginPlay();
	SphereComponent->OnComponentBeginOverlap.AddDynamic(this,&ABaseWeapon::OnSphereBeginOverlap);
	
}

void ABaseWeapon::OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp,Warning,TEXT("Overlap"));
	AGPROGCharacter* Player = Cast<AGPROGCharacter>(OtherActor);
	if(Player)
	{
		AttachWeapon(Player);
	}
	
}



// Called every frame
void ABaseWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

