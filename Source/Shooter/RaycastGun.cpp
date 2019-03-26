// Fill out your copyright notice in the Description page of Project Settings.

#include "RaycastGun.h"
#include "Health.h"
#include "Shooter.h"
#include "ShooterCharacter.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"


void ARaycastGun::PerformFire()
{

	if (!CanFire())
		return;
	PerformRaycast();

	Super::PerformFire();
}



void ARaycastGun::PerformRaycast()
{
	AActor* myOwner = GetOwner();
	if (!myOwner)
		return;

	FVector eyeLocation;
	FRotator eyeRotation;
	myOwner->GetActorEyesViewPoint(eyeLocation, eyeRotation);
	CreateRandomOffset(eyeRotation);

	FHitResult hitResult;
	FVector endPos = (eyeRotation.Vector() * shootDistance) + eyeLocation;
	FCollisionQueryParams params = FCollisionQueryParams();
	params.AddIgnoredActor(this);
	params.AddIgnoredActor(myOwner);
	if (GetWorld()->LineTraceSingleByChannel(hitResult, eyeLocation, endPos, ECC_Visibility, params))
	{
		if (Role < ROLE_Authority)
		{
			SimulateHit(hitResult);
			ServerValidateHit(hitResult, eyeRotation.Vector());
		}
		else
			ValidateHit(hitResult, eyeRotation.Vector());
	}


}

void ARaycastGun::ValidateHit(const FHitResult & hitResult, FVector_NetQuantize dir)
{
	//Shot was valid, see if it hit anything: 
	SimulateHit(hitResult);
	if (hitResult.GetActor())
	{
		FBox hitBox = hitResult.GetActor()->GetComponentsBoundingBox();
		//20% wiggle room
		FVector hitBoxExtents = hitBox.GetExtent() * 1.2f;
		FVector hitBoxCenter = hitBox.GetCenter();
		if ((FMath::Abs(hitResult.Location.X - hitBoxCenter.X) < hitBoxExtents.X) &&
			(FMath::Abs(hitResult.Location.Y - hitBoxCenter.Y) < hitBoxExtents.Y) &&
			(FMath::Abs(hitResult.Location.Z - hitBoxCenter.Z) < hitBoxExtents.Z))
		{

			//Object was hit. Damage it, apply force to it, show hit effects: 
			if (hitResult.GetActor()->IsRootComponentMovable())
			{
				UStaticMeshComponent* meshRootComponent = Cast<UStaticMeshComponent>(hitResult.GetActor()->GetRootComponent());
				if (meshRootComponent)
					meshRootComponent->AddForce(dir * impactForce);
			}
			if (UHealth* health = hitResult.GetActor()->FindComponentByClass<UHealth>())
			{
				health->Damage(damage);
			}
		}
	}
}

void ARaycastGun::ServerValidateHit_Implementation(const FHitResult & hitResult, FVector_NetQuantize dir)
{
	ValidateHit(hitResult, dir);
}

bool ARaycastGun::ServerValidateHit_Validate(const FHitResult & hitResult, FVector_NetQuantize dir)
{
	return true;
}

void ARaycastGun::CreateRandomOffset(FRotator& rotator)
{
	if (!character)
		return;
	float inaccuracy = character->IsFocused() ? FocusInaccuracy : hipFireInaccuracy;
	rotator += FRotator(FMath::FRandRange(-inaccuracy / 2.f, inaccuracy / 2.f),
		FMath::FRandRange(-inaccuracy / 2.f, inaccuracy / 2.f), 0);
}

