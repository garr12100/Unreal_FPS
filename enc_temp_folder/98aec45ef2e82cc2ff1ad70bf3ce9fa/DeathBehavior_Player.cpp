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
			if (Ragdoll_BP)
			{
				if (USkeletalMeshComponent* mesh3P = character->GetMesh3P())
				{
					ASkeletalMeshActor* ragdoll = GetWorld()->SpawnActor<ASkeletalMeshActor>(Ragdoll_BP, mesh3P->GetComponentLocation(), mesh3P->GetComponentRotation());
					FPoseSnapshot pose;
					mesh3P->SnapshotPose(pose);
					character->SetLifeSpan(.01f);
					ragdoll->GetSkeletalMeshComponent()->MasterPoseComponent;
					//ragdoll->GetSkeletalMeshComponent()->GetAnimInstance()->Set;

					//mesh3P->Bone

					//int32 NumBones = ragdoll->GetSkeletalMeshComponent()->GetNumBones();

					//for (int32 i = 0; i < NumBones; ++i)
					//{
						//FName const BoneName = ragdoll->GetSkeletalMeshComponent()->GetBoneName(i);


						//FTransform Transform(mesh3P->GetBoneTra, SkeletonDataActual.Locations[i], FVector(SkeletonDataActual.Scale));

						ragdoll->GetSkeletalMeshComponent()->BoneSpaceTransforms = mesh3P->BoneSpaceTransforms;// (BoneName, mesh3P->GetBoneTransform(i), EBoneSpaces::WorldSpace);
					//}
					//PoseableMeshComponent->SetWorldScale3D(FVector(SkeletonDataActual.Scale));


					//ragdoll->GetSkeletalMeshComponent()->AnimClass->pose
					//mesh->snaps;
					//mesh->SetSimulatePhysics(true);
				}
			}
		}
	}
}
