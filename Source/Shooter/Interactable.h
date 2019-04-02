// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interactable.generated.h"

UCLASS()
class SHOOTER_API AInteractable : public AActor
{
	GENERATED_BODY()
	
public:	

	AInteractable();

	UPROPERTY(EditAnywhere)
		FString prefix = "Interact with";

	UPROPERTY(EditAnywhere)
		FString name = "???";

	UFUNCTION(BlueprintCallable)
		FString GetInteractionString();

};
