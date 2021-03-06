// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ShooterPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTER_API AShooterPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public: 

	/** Perform a respawn using GameMode */
	void Respawn();

	/** Perform respawn on Server */
	UFUNCTION(Server, Reliable, WithValidation)
		void ServerRespawn();
	virtual bool ServerRespawn_Validate();
	virtual void ServerRespawn_Implementation();

};
