// Fill out your copyright notice in the Description page of Project Settings.

#include "KillableObject.h"
#include "Health.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
AKillableObject::AKillableObject()
{
	PrimaryActorTick.bCanEverTick = false;

	bReplicates = true;
	bReplicateMovement = true;
	mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	SetRootComponent(mesh);
	health = CreateDefaultSubobject<UHealth>(TEXT("Health"));

}

