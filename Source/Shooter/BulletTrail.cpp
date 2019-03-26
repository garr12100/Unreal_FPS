// Fill out your copyright notice in the Description page of Project Settings.

#include "BulletTrail.h"
#include "Classes/Components/SceneComponent.h"
#include "Classes/Components/StaticMeshComponent.h"

// Sets default values
ABulletTrail::ABulletTrail()
{
	PrimaryActorTick.bCanEverTick = false;
	root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = root;
	mesh->SetupAttachment(root);

}

// Called when the game starts or when spawned
void ABulletTrail::BeginPlay()
{
	Super::BeginPlay();
	SetLifeSpan(lifetime);
}



