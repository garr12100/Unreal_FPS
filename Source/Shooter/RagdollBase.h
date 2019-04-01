// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/SkeletalMeshActor.h"
#include "RagdollBase.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTER_API ARagdollBase : public ASkeletalMeshActor
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(ReplicatedUsing = OnRep_Mesh)
		class USkeletalMesh* SkeletalMesh_Replicated;

	UFUNCTION()
		virtual void OnRep_Mesh();

public:

	ARagdollBase();

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	void SetSkeletalMesh(USkeletalMesh* newMesh);


};
