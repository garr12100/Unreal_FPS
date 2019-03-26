// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "KillableObject.generated.h"

UCLASS()
class SHOOTER_API AKillableObject : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AKillableObject();

public:	

	/**	Mesh Component */
	UPROPERTY(EditAnywhere)
		class UStaticMeshComponent* mesh;

	/**	Health component*/
	UPROPERTY(EditAnywhere)
		class UHealth* health;

};
