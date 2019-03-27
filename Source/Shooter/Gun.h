// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Gun.generated.h"

USTRUCT(BlueprintType)
struct FBoneMultiplier
{
	GENERATED_BODY()

public:

	FBoneMultiplier();

	UPROPERTY(EditAnywhere)
		FName boneName;

	UPROPERTY(EditAnywhere)
		float damageMultiplier;
};

UCLASS()
class SHOOTER_API AGun : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AGun();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//Reference to owning character
	class AShooterCharacter* character;

	/**Handles Actual firing of gun*/
	virtual void PerformFire();

	/**Is the gun able to fire, based on cooldown?*/
	virtual bool CanFire();

	/**Simulate a bullet hit via effects. */
	void SimulateHit(const FHitResult& hitResult);

	/**Get location of gun's Muzzle socket. */
	FORCEINLINE FVector GetMuzzleLocation() const { return FP_Gun->GetSocketLocation("Muzzle"); }

	/** Calculate damage based on bone hit */
	float GetDamage(const FHitResult& hit);

private:
	//Time Stamp of last Gun Fire
	float lastShotTime = 0.f;

	//Is Trigger Pulled? 
	bool triggerPulled = false;

	/** Spawn Particle for bullet impact */
	void SpawnImpactParticle(const FHitResult& hitResult);

	/** Spawn Bullet Trail/Beam Object*/
	void SpawnBulletTrail(FVector_NetQuantize endPos);

	/** Play Sound when bullet hits*/
	void PlayImpactSound();

	/**Play all effects representing bullet hit */
	void PlayHitEffects(const FHitResult& hitResult);

	/**Simulate a bullet hit via effects, Multicast */
	UFUNCTION(NetMulticast, Unreliable)
		void MulticastSimulateHit(const FHitResult & hitResult);

	/**Implementation of simulating a bullet hit via effects, Multicast */
	virtual void MulticastSimulateHit_Implementation(const FHitResult & hitResult);

	/** Play Sound and Particle effects when firing */
	virtual void PlayFireEffects();

	/** Fire on Server. */
	UFUNCTION(Server, Reliable, WithValidation)
		void ServerFire();

	/** Fire on Server Implementation. */
	virtual void ServerFire_Implementation();

	/** Fire on Server Validation. */
	virtual bool ServerFire_Validate();

	/** Fire multicast - Simulates effects to clients. */
	UFUNCTION(NetMulticast, Unreliable)
		void MulticastFire();

	/** Fire multicast Implementation - Simulates effects to clients. */
	virtual void MulticastFire_Implementation();


public:

	/** Root Scene Component */
	UPROPERTY(EditAnywhere)
		class USceneComponent* RootComp;

	/** Sound to play each time we fire */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		class USoundBase* FireSound;

	/** How much Damage this gun does to targets */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Projectile)
		float damage = 5.f;

	/** Multipliers for specific bones */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Projectile)
		TArray<FBoneMultiplier> boneMultipliers;

	/** Required cooldown in between  */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Projectile)
		float cooldown = 0.f;

	/** Is this an automatic gun? */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Projectile)
		bool automatic = false;

	/** Innacurracy factor during hipfire */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Projectile)
		float hipFireInaccuracy = 2.f;

	/** Innacurracy factor during focus fire */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Projectile)
		float FocusInaccuracy = 0.f;

	/** Zoom amount - Camera's Field of View while zoomed. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		float ZoomFOV = 60.f;

	/** Gun mesh: 1st person view (seen only by self) */
	UPROPERTY(EditAnywhere, Category = Mesh)
		class USkeletalMeshComponent* FP_Gun;

	/** Particle used for Muzzle Flash */
	UPROPERTY(VisibleDefaultsOnly, Category = Projectile)
		class UParticleSystemComponent* MuzzleFlash;

	/** Particle used for Bullet Impact */
	UPROPERTY(VisibleDefaultsOnly, Category = Projectile)
		class UParticleSystemComponent* ImpactParticle;

	/** Blueprint for the WeaponPickup version of this gun */
	UPROPERTY(EditAnywhere, Category = Gameplay)
		TSubclassOf<class AWeaponPickup> WeaponPickupBluePrint;

	/** Blueprint of the bullet trail. */
	UPROPERTY(EditAnywhere, Category = Projectile)
		TSubclassOf<class ABulletTrail> BulletTrail_BP;

	/** Sets Owner */
	virtual void SetOwner(AActor* NewOwner) override;

	/** Fire the gun. */
	virtual void Fire();

	/** Pull the trigger - will attempt to fire until it is released. */
	virtual void PullTrigger();

	/**Release the trigger. */
	virtual void ReleaseTrigger();

	/** Drop the gun and spawn a WeaponPickup in its place. */
	virtual void Drop();

};
