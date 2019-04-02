// Fill out your copyright notice in the Description page of Project Settings.

#include "Interactor.h"
#include "Interactable.h"
#include "WeaponPickup.h"
#include "ShooterCharacter.h"
#include "Shooter.h"
#include "Net/UnrealNetwork.h"

// Sets default values for this component's properties
UInteractor::UInteractor()
{
	PrimaryComponentTick.bCanEverTick = false;

}

void UInteractor::BeginPlay()
{
	Super::BeginPlay();

}



void UInteractor::SetInteractingWith(AInteractable * _interactWith)
{
	interactingWith = _interactWith;
	if (character)
	{
		if (interactingWith)
			character->OnInteractable(FText::FromString(interactingWith->GetInteractionString()));
		else
			character->OnInteractableNull();
	}
}

void UInteractor::Interact()
{
	if (character->Role < ROLE_Authority)
	{
		ServerInteract();
	}
	else
	{
		//Perform various types of interactions
		if (!interactingWith || !character || interactingWith->IsActorBeingDestroyed())
			return;
		//Pick up weapons
		if (AWeaponPickup* wp = Cast<AWeaponPickup>(interactingWith))
		{
			wp->SpawnGun(character);
		}
	}
}

bool UInteractor::ServerInteract_Validate()
{
	return true;
}

void UInteractor::ServerInteract_Implementation()
{
	Interact();
}

