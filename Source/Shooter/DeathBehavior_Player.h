// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DeathBehavior.h"
#include "DeathBehavior_Player.generated.h"

/**
 * 
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class SHOOTER_API UDeathBehavior_Player : public UDeathBehavior
{
	GENERATED_BODY()

public:
	/** Blueprint of Ragoll to spawn. */
	UPROPERTY(EditAnywhere)
		TSubclassOf<class ARagdollBase> Ragdoll_BP;

		virtual void HandleDeath() override;

		UFUNCTION(Server, Reliable, WithValidation)
		void HandleDeathServer();
		virtual void HandleDeathServer_Implementation();
		virtual bool HandleDeathServer_Validate();


private:

	/** Spawn Ragoll in place */
	void SpawnRagdoll(USkeletalMeshComponent* mesh3P);


	/** SetSkeletalMesh isn't replicated automatically - multicast it. */
	//UFUNCTION(NetMulticast, Reliable)
	//	void MulticastSetRagdollMesh(class ASkeletalMeshActor* ragdoll, class USkeletalMesh* newMesh);
	//void MulticastSetRagdollMesh_Implementation(class ASkeletalMeshActor* ragdoll, class USkeletalMesh* newMesh);
	
};
