// Fill out your copyright notice in the Description page of Project Settings.

#include "WeaponPickup.h"
#include "Shooter.h"
#include "Components/BoxComponent.h"
#include "Interactor.h"
#include "Gun.h"
#include "Net/UnrealNetwork.h"
#include "ShooterCharacter.h"

// Sets default values
AWeaponPickup::AWeaponPickup()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;
	bReplicateMovement = true;

	WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	CollisionComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Collider"));
	RootComponent = WeaponMesh;
	CollisionComp->SetupAttachment(RootComponent);

	OnActorBeginOverlap.AddDynamic(this, &AWeaponPickup::OnOverlapBegin);
	OnActorEndOverlap.AddDynamic(this, &AWeaponPickup::OnOverlapEnd);


}

// Called when the game starts or when spawned
void AWeaponPickup::BeginPlay()
{
	Super::BeginPlay();
	
}



void AWeaponPickup::OnOverlapBegin(AActor * overlappedActor, AActor * otherActor)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Enter")));
	if(UInteractor* interactor = otherActor->FindComponentByClass<UInteractor>())
	{
		interactor->interactingWith = this;
	}

}

void AWeaponPickup::OnOverlapEnd(AActor * overlappedActor, AActor * otherActor)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Exit")));
	if (UInteractor* interactor = otherActor->FindComponentByClass<UInteractor>())
	{
		if(interactor->interactingWith == this)
			interactor->interactingWith = nullptr;
	}
}

void AWeaponPickup::SpawnGun(APawn* pawn)
{
	if (GunBlueprint)
	{
		AShooterCharacter* character = Cast<AShooterCharacter>(pawn);
		if (character)
			character->SetGunFromBlueprint(GunBlueprint);
	}

	if (DestroyOnPickup)
		Destroy();

}



