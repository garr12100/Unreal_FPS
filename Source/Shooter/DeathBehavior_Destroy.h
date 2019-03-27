// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DeathBehavior.h"
#include "DeathBehavior_Destroy.generated.h"

/**
 * 
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class SHOOTER_API UDeathBehavior_Destroy : public UDeathBehavior
{
	GENERATED_BODY()
	
		virtual void HandleDeath() override;


};
