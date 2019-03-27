// Fill out your copyright notice in the Description page of Project Settings.

#include "DeathBehavior_Destroy.h"
#include "Shooter.h"


void UDeathBehavior_Destroy::HandleDeath()
{
	GetOwner()->Destroy();
}
