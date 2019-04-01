// Fill out your copyright notice in the Description page of Project Settings.

#include "RagdollBase.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/SkeletalMesh.h"
#include "Net/UnrealNetwork.h"



ARagdollBase::ARagdollBase()
{
	PrimaryActorTick.bCanEverTick = false;

	bReplicates = true;
	bReplicateMovement = true;
}

void ARagdollBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ARagdollBase, SkeletalMesh_Replicated);

}

void ARagdollBase::OnRep_Mesh()
{
	GetSkeletalMeshComponent()->SetSkeletalMesh(SkeletalMesh_Replicated);
}

void ARagdollBase::SetSkeletalMesh(USkeletalMesh * newMesh)
{
	SkeletalMesh_Replicated = newMesh;
	if (Role == ROLE_Authority)
		OnRep_Mesh();
}
