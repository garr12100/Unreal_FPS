// Fill out your copyright notice in the Description page of Project Settings.

#include "Gun.h"
#include "Kismet/GameplayStatics.h"
#include "Animation/AnimInstance.h"
#include "ShooterCharacter.h"
#include "Shooter.h"
#include "Kismet/KismetMathLibrary.h"
#include "WeaponPickup.h"
#include "Classes/Particles/ParticleSystemComponent.h"
#include "BulletTrail.h"



FBoneMultiplier::FBoneMultiplier()
{
	boneName = "";
	damageMultiplier = 1.0f;
}

// Sets default values
AGun::AGun()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	bReplicateMovement = true;
	bAlwaysRelevant = true;

	RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = RootComp;

	// Create a gun mesh component
	FP_Gun = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FP_Gun"));
	FP_Gun->bCastDynamicShadow = false;
	FP_Gun->CastShadow = false;
	FP_Gun->SetupAttachment(RootComponent);

	MuzzleFlash = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("MuzzleFlash"));
	MuzzleFlash->SetupAttachment(FP_Gun);
	MuzzleFlash->SetAutoActivate(false);

	ImpactParticle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ImpactBP"));
	ImpactParticle->SetupAttachment(RootComp);
	ImpactParticle->SetAutoActivate(false);

}

void AGun::BeginPlay()
{
	Super::BeginPlay();
}


void AGun::PerformFire()
{
	PlayFireEffects();
}

bool AGun::CanFire()
{
	float t = UGameplayStatics::GetRealTimeSeconds(GetWorld());
	if ((t - lastShotTime) > cooldown)
		return true;
	return false;
}

void AGun::SpawnImpactParticle(const FHitResult& hitResult)
{
	if (ImpactParticle && hitResult.GetActor())
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactParticle->Template, FTransform(-1 * hitResult.Normal.Rotation(), hitResult.ImpactPoint, FVector(1, 1, 1)), true);
	}
}

void AGun::SpawnBulletTrail(FVector_NetQuantize endPos)
{
	if (BulletTrail_BP)
	{
		UWorld* world = GetWorld();
		if (world)
		{
			FActorSpawnParameters params;
			FVector spawnAt = GetMuzzleLocation();
			float distance = FVector::Distance(spawnAt, endPos);
			//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Hit Dist: %s"), *(FString::SanitizeFloat(distance))));
			FVector dir = endPos - spawnAt;
			dir.Normalize();
			FRotator spawnRotation = dir.Rotation().Add(-90, 0, 0);
			ABulletTrail* trail = world->SpawnActor<ABulletTrail>(BulletTrail_BP, spawnAt, spawnRotation, params);
			trail->SetActorScale3D(FVector(1.f, 1.f, distance / trail->meshLength));
		}
	}
}

void AGun::PlayImpactSound()
{

}

void AGun::PlayHitEffects(const FHitResult & hitResult)
{

	SpawnImpactParticle(hitResult);
	SpawnBulletTrail(hitResult.Location);
	PlayImpactSound();

}

void AGun::SimulateHit(const FHitResult & hitResult)
{
	if (Role < ROLE_Authority)
	{
		PlayHitEffects(hitResult);
	}
	else
	{
		MulticastSimulateHit(hitResult);
	}
}

float AGun::GetDamage(const FHitResult& hit)
{
	float _damage = damage;
	for (int i = 0; i < boneMultipliers.Num(); i++)
	{
		if (hit.BoneName == boneMultipliers[i].boneName)
		{
			_damage *= boneMultipliers[i].damageMultiplier;
			break;
		}
	}

	return _damage;
}

void AGun::MulticastSimulateHit_Implementation(const FHitResult & hitResult)
{
	PlayHitEffects(hitResult);
}


void AGun::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (automatic && triggerPulled)
		Fire();
}

void AGun::SetOwner(AActor * NewOwner)
{
	Super::SetOwner(NewOwner);

	character = Cast<AShooterCharacter>(NewOwner);

}

void AGun::PlayFireEffects()
{
	if (!CanFire())
		return;
	lastShotTime = UGameplayStatics::GetRealTimeSeconds(GetWorld());

	if (FireSound != NULL)
	{
		UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
	}
	if (MuzzleFlash != NULL)
	{

		UGameplayStatics::SpawnEmitterAttached(MuzzleFlash->Template, FP_Gun, "MuzzleFlash", GetMuzzleLocation(), FRotator::ZeroRotator, MuzzleFlash->GetComponentScale(), EAttachLocation::KeepWorldPosition, true);
	}

	if (character)
	{
		character->PlayFireAnimation();
	}
}

void AGun::Fire()
{
	PerformFire();

	if (Role < ROLE_Authority)
	{
		ServerFire();
	}
	else
	{
		MulticastFire();
	}

}

void AGun::ServerFire_Implementation()
{
	MulticastFire();
}

bool AGun::ServerFire_Validate()
{
	return true;
}

void AGun::MulticastFire_Implementation()
{
	PlayFireEffects();
}

void AGun::PullTrigger()
{
	Fire();
	triggerPulled = true;
}

void AGun::ReleaseTrigger()
{
	triggerPulled = false;
}

void AGun::Drop()
{

	if (WeaponPickupBluePrint)
	{
		UWorld* world = GetWorld();
		if (world)
		{
			FActorSpawnParameters params;
			params.Owner = NULL;
			FVector spawnAt = GetActorLocation();
			FRotator spawnRotation = UKismetMathLibrary::RandomRotator(true);
			world->SpawnActor<AWeaponPickup>(WeaponPickupBluePrint, spawnAt, spawnRotation, params);
		}

		Destroy();

	}

}


