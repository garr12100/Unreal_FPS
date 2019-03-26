// Fill out your copyright notice in the Description page of Project Settings.

#include "Interactor.h"
#include "WeaponPickup.h"
#include "ShooterCharacter.h"
#include "Net/UnrealNetwork.h"

// Sets default values for this component's properties
UInteractor::UInteractor()
{
	PrimaryComponentTick.bCanEverTick = false;

}

void UInteractor::SetInteractingWith(AActor * _interactWith)
{
	interactingWith = _interactWith;
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
		if (!interactingWith || !character)
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

