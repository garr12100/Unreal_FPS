// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectileGun.h"

void AProjectileGun::PerformFire()
{
	if (!CanFire())
		return;
	// try and fire a projectile
	//if (ProjectileClass != NULL)
	//{
	UWorld* const World = GetWorld();
	if (World != NULL)
	{

		//const FRotator SpawnRotation = GetControlRotation();
		//// MuzzleOffset is in camera space, so transform it to world space before offsetting from the character location to find the final muzzle position
		//const FVector SpawnLocation = ((FP_MuzzleLocation != nullptr) ? FP_MuzzleLocation->GetComponentLocation() : GetActorLocation()) + SpawnRotation.RotateVector(GunOffset);

		////Set Spawn Collision Handling Override
		//FActorSpawnParameters ActorSpawnParams;
		//ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

		//// spawn the projectile at the muzzle
		//World->SpawnActor<AShooterProjectile>(ProjectileClass, SpawnLocation, SpawnRotation, ActorSpawnParams);

	}
	Super::PerformFire();

	//}
	//}
}