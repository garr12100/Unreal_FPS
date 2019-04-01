// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SpectatorPawn.h"
#include "SpectatorPawn_Respawnable.generated.h"

class UInputComponent;


/**
 * 
 */
UCLASS()
class SHOOTER_API ASpectatorPawn_Respawnable : public ASpectatorPawn
{
	GENERATED_BODY()

protected:


public:

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/** Respawn function binded to Respawn input action */
	void Respawn();

};
