// Fill out your copyright notice in the Description page of Project Settings.

#include "SpectatorPawn_Respawnable.h"
#include "Components/InputComponent.h"
#include "Shooter.h"
#include "ShooterPlayerController.h"




void ASpectatorPawn_Respawnable::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);


	// set up gameplay key bindings
	check(PlayerInputComponent);


	// Bind Respawn event
	PlayerInputComponent->BindAction("Respawn", IE_Pressed, this, &ASpectatorPawn_Respawnable::Respawn);

}

void ASpectatorPawn_Respawnable::Respawn()
{
	if (AShooterPlayerController* pc = Cast<AShooterPlayerController>(GetController()))
	{
		pc->Respawn();
	}
}
