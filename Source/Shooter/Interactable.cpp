// Fill out your copyright notice in the Description page of Project Settings.

#include "Interactable.h"


AInteractable::AInteractable()
{

}

FString AInteractable::GetInteractionString()
{
	return prefix + " " + name;
}
