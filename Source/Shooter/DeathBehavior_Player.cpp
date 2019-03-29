// Fill out your copyright notice in the Description page of Project Settings.

#include "DeathBehavior_Player.h"
#include "ShooterCharacter.h"
#include "GameFramework/PlayerState.h"
#include "GameFramework/PlayerController.h"
#include "Components/SkeletalMeshComponent.h"
#include "Classes/Animation/SkeletalMeshActor.h"
//#include "Classes/Animation/.h"

void UDeathBehavior_Player::HandleDeath()
{
	if (GetOwner()->Role == ROLE_Authority)
	{
		if (AShooterCharacter* character = Cast<AShooterCharacter>(GetOwner()))
		{
			character->SetStateSpectator();
			character->SetGun(NULL);
			SpawnRagdoll(character->GetMesh3P());
			character->SetLifeSpan(.001f);
		}
	}
}

void UDeathBehavior_Player::SpawnRagdoll(USkeletalMeshComponent* mesh3P)
{
	if (Ragdoll_BP)
	{
		if (mesh3P)
		{
			ASkeletalMeshActor* ragdoll = GetWorld()->SpawnActor<ASkeletalMeshActor>(Ragdoll_BP, mesh3P->GetComponentLocation(), mesh3P->GetComponentRotation());
			FPoseSnapshot pose;
			mesh3P->SnapshotPose(pose);
		}
	}
}
