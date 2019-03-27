// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "ShooterCharacter.h"
#include "ShooterProjectile.h"
#include "Shooter.h"
#include "Engine/Classes/Components/StaticMeshComponent.h"
#include "Engine/Engine.h"
#include "Engine/World.h"
#include "Health.h"
#include "Interactor.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/InputSettings.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Gun.h"
#include "CharacterCameraController.h"
#include "Net/UnrealNetwork.h"

DEFINE_LOG_CATEGORY_STATIC(LogFPChar, Warning, All);

//////////////////////////////////////////////////////////////////////////
// AShooterCharacter

AShooterCharacter::AShooterCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->RelativeLocation = FVector(-39.56f, 1.75f, 64.f); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	//Create Camera Controller Component
	CameraControllerComponent = CreateDefaultSubobject<UCharacterCameraController>(TEXT("CameraController"));

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetupAttachment(FirstPersonCameraComponent);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	Mesh1P->RelativeRotation = FRotator(1.9f, -19.19f, 5.2f);
	Mesh1P->RelativeLocation = FVector(-0.5f, -4.4f, -155.7f);

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	Mesh3P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh3P"));
	Mesh3P->SetOwnerNoSee(true);
	Mesh3P->SetupAttachment(FirstPersonCameraComponent);
	Mesh3P->bCastDynamicShadow = true;
	Mesh3P->CastShadow = true;

	// Create an Interactor component
	Interactor = CreateDefaultSubobject<UInteractor>(TEXT("Interactor"));

	// Create an Interactor component
	Health = CreateDefaultSubobject<UHealth>(TEXT("Health"));

	OnDestroyed.AddDynamic(this, &AShooterCharacter::HandleDestroyed);

}

void AShooterCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AShooterCharacter, Focused);
	DOREPLIFETIME(AShooterCharacter, gun);

}

void AShooterCharacter::HandleDestroyed(AActor * actor)
{
	if (gun)
		gun->Destroy();
}

void AShooterCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	Interactor->character = this;

	CameraControllerComponent->Camera = FirstPersonCameraComponent;

	SetGunFromBlueprint(defaultGun_BP);
	//SetGun(gun);

	defaultSpeed = GetCharacterMovement()->MaxWalkSpeed;

}

void AShooterCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// set up gameplay key bindings
	check(PlayerInputComponent);

	// Bind jump events
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	// Bind fire event
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &AShooterCharacter::OnFire);
	PlayerInputComponent->BindAction("Fire", IE_Released, this, &AShooterCharacter::OnStopFire);

	// Bind Interact Event
	PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &AShooterCharacter::Interact);

	// Bind Focus Events
	PlayerInputComponent->BindAction("Focus", IE_Pressed, this, &AShooterCharacter::Focus);
	PlayerInputComponent->BindAction("Focus", IE_Released, this, &AShooterCharacter::Unfocus);


	// Bind movement events
	PlayerInputComponent->BindAxis("MoveForward", this, &AShooterCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AShooterCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &AShooterCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AShooterCharacter::LookUpAtRate);
}

void AShooterCharacter::OnFire()
{
	if (Paused)
		return;
	if (gun)
	{
		gun->PullTrigger();
	}
}


void AShooterCharacter::OnStopFire()
{

	if (gun)
	{
		gun->ReleaseTrigger();
	}
}

void AShooterCharacter::PlayFireAnimation()
{
	// try and play a firing animation if specified
	bool local = IsLocallyControlled();
	UAnimMontage* FireAnimation = local ? FireAnimation1P : FireAnimation3P;
	USkeletalMeshComponent* targetMesh = local ? Mesh1P : Mesh3P;
	if (FireAnimation && targetMesh)
	{

		UAnimInstance* AnimInstance = targetMesh->GetAnimInstance();
		if (AnimInstance != NULL)
		{
			float val = AnimInstance->Montage_Play(FireAnimation, 1.f);
			AnimInstance->Montage_Play(FireAnimation, 1.f);
		}
	}
}


void AShooterCharacter::MoveForward(float Value)
{
	if (Paused)
		return;
	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorForwardVector(), Value);
	}
}

void AShooterCharacter::MoveRight(float Value)
{
	if (Paused)
		return;
	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorRightVector(), Value);
	}
}

void AShooterCharacter::TurnAtRate(float Rate)
{
	if (Paused)
		return;
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AShooterCharacter::LookUpAtRate(float Rate)
{
	if (Paused)
		return;
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}


FVector AShooterCharacter::GetPawnViewLocation() const
{
	if (FirstPersonCameraComponent)
		return FirstPersonCameraComponent->GetComponentLocation();
	return Super::GetPawnViewLocation();
}


void AShooterCharacter::SetGun(AGun* newGun)
{
	if (!newGun)
		return;
	DetachGun();
	gun = newGun;
	AttachGun();
}

void AShooterCharacter::SetGunFromBlueprint(TSubclassOf<class AGun> newGunBP)
{
	if (!newGunBP || Role != ROLE_Authority)
		return;
	
	if (UWorld* world = GetWorld())
	{
		AGun* newGun = world->SpawnActor<AGun>(newGunBP);
		newGun->SetReplicates(true);
		SetGun(newGun);
	}

}

void AShooterCharacter::OnRep_Gun()
{
	if (gun)
		AttachGun();
}

void AShooterCharacter::DetachGun()
{
	if (gun)
	{
		gun->DetachFromActor(FDetachmentTransformRules(EDetachmentRule::KeepWorld, true));
		gun->Drop();
	}
}

void AShooterCharacter::AttachGun()
{
	gun->SetOwner(this);
	bool local = IsLocallyControlled();
	if (IsLocallyControlled())
		gun->AttachToComponent(Mesh1P, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("RightHand"));
	else
		gun->AttachToComponent(Mesh3P, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("RightHand"));

}


void AShooterCharacter::Interact()
{
	if (Paused)
		return;
	if (Interactor)
		Interactor->Interact();
}

void AShooterCharacter::Focus()
{
	if (Paused)
		return;
	if (Role < ROLE_Authority)
		ServerFocus(true);
	Focused = true;
	CameraControllerComponent->Zoom(true, gun ? gun->ZoomFOV : -1.f);
	GetCharacterMovement()->MaxWalkSpeed = defaultSpeed * FocusedSpeedMultiplier;
}

void AShooterCharacter::ServerFocus_Implementation(bool _focused)
{
	if (_focused)
		Focus();
	else
		Unfocus();
}

bool AShooterCharacter::ServerFocus_Validate(bool _focused)
{
	return true;
}

void AShooterCharacter::Unfocus()
{
	if (Role < ROLE_Authority)
		ServerFocus(false);
	Focused = false;
	CameraControllerComponent->Zoom(false);
	GetCharacterMovement()->MaxWalkSpeed = defaultSpeed;

}

bool AShooterCharacter::IsFocused() const
{
	return Focused;
}

void AShooterCharacter::SetPaused(bool on)
{
	Paused = on;
	OnStopFire();
	Unfocus();
}
