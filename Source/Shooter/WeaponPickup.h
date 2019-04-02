// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Interactable.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WeaponPickup.generated.h"

UCLASS()
class SHOOTER_API AWeaponPickup : public AInteractable
{

	GENERATED_BODY()

public:

	 //Sets default values for this actor's properties
	AWeaponPickup();

	/** Mesh Component */
	UPROPERTY(EditAnywhere, Category = Components)
		class UStaticMeshComponent* WeaponMesh;

	/** Collision Component to check overlapping. */
	UPROPERTY(EditAnywhere, Category = Components)
		class UBoxComponent* CollisionComp;

	/** Blueprint of Gun represented by this pickup. */
	UPROPERTY(EditAnywhere)
		TSubclassOf<class AGun> GunBlueprint;

	/** Should the pickup be destroyed when picked up?  */
	UPROPERTY(EditAnywhere)
		bool DestroyOnPickup = true;

protected:

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


public:

	/** Handle an overlap beginning */ 
	UFUNCTION()
		void OnOverlapBegin(class AActor* overlappedActor, class AActor* otherActor);
	/** Handle an overlap Ending */
	UFUNCTION()
		void OnOverlapEnd(class AActor* overlappedActor, class AActor* otherActor);

	/** Spawn an actual gun from this Pickup. */
		void SpawnGun(APawn* pawn);



};
