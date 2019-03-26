// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Gun.h"
#include "RaycastGun.generated.h"

/**
 *
 */
UCLASS()
class SHOOTER_API ARaycastGun : public AGun
{
	GENERATED_BODY()


private:

	/** Create offset to a rotator based on inaccuracy */
	void CreateRandomOffset(FRotator& rotator);

	/** Validate if anything was hit, and apply hit effects. */
	void ValidateHit(const FHitResult& hitResult, FVector_NetQuantize dir);

	/** Validate if anything was hit on the Server. */
	UFUNCTION(Server, Reliable, WithValidation)
		void ServerValidateHit(const FHitResult& hitResult, FVector_NetQuantize dir);

	/** Validate if anything was hit on the Server, Implementation. */
	virtual void ServerValidateHit_Implementation(const FHitResult& hitResult, FVector_NetQuantize dir);

	/** Validate if anything was hit on the Server, Validation. */
	virtual bool ServerValidateHit_Validate(const FHitResult& hitResult, FVector_NetQuantize dir);

protected:

	virtual void PerformFire() override;

	/** Perform the Raycast that represents the bullet. */
	void PerformRaycast();


public:

	/** How far can this gun shoot? */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Projectile)
		float shootDistance = 5000.f;

	/** Force to apply to hit objects. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Projectile)
		float impactForce = 50000000.f;

};
