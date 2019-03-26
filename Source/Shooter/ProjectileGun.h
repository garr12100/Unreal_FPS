// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Gun.h"
#include "ProjectileGun.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTER_API AProjectileGun : public AGun
{
	GENERATED_BODY()

public:
	virtual void PerformFire() override;

	///** Projectile class to spawn */
//UPROPERTY(EditDefaultsOnly, Category = Projectile)
//	TSubclassOf<class AShooterProjectile> ProjectileClass;


};
