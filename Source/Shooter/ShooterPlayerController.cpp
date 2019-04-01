// Fill out your copyright notice in the Description page of Project Settings.

#include "ShooterPlayerController.h"
#include "Shooter.h"

void AShooterPlayerController::Respawn()
{
	ServerRespawn();
}


bool AShooterPlayerController::ServerRespawn_Validate()
{
	return true;
}

void AShooterPlayerController::ServerRespawn_Implementation()
{
	//MulticastRespawn();
	if (UWorld* world = GetWorld())
	{
		if (AGameModeBase* gameModeBase = world->GetAuthGameMode())
		{
			//if (AGameMode* gameMode = Cast<AGameMode>(gameModeBase))
			gameModeBase->RestartPlayer(this);
		}
	}
}