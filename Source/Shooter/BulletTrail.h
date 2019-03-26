// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BulletTrail.generated.h"

UCLASS()
class SHOOTER_API ABulletTrail : public AActor
{
	GENERATED_BODY()
	
public:	

	// Sets default values for this actor's properties
	ABulletTrail();

	/** Root Scene component */
	UPROPERTY(EditAnywhere, Category = Mesh)
		class USceneComponent* root;

	/** Mesh component */
	UPROPERTY(EditAnywhere, Category = Mesh)
		class UStaticMeshComponent* mesh;

	/** Length of the meshed - used to normalize mesh to 1 unit size. */
	UPROPERTY(EditAnywhere, Category = Mesh)
		float meshLength = 251.8f;

	/** How long the Trail is visible */
	UPROPERTY(EditAnywhere)
		float lifetime = 0.1f;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


};
