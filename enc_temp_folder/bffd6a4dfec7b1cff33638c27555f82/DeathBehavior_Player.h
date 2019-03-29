// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DeathBehavior.h"
#include "DeathBehavior_Player.generated.h"

/**
 * 
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class SHOOTER_API UDeathBehavior_Player : public UDeathBehavior
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere)
		TSubclassOf<class ASkeletalMeshActor> Ragdoll_BP;

		virtual void HandleDeath() override;

	
};
