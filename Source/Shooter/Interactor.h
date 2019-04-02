// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Interactor.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SHOOTER_API UInteractor : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInteractor();

	//void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	/** Actor being interacted with. */
	//UPROPERTY(Replicated)
	class AInteractable* interactingWith;

	/** Owning Character */
	UPROPERTY()
		class AShooterCharacter* character;

	/** Set what this interactor is interacting with. */
	void SetInteractingWith(class AInteractable* _interactWith);

	/** Perform interaction with various types of objects. */
	UFUNCTION(BlueprintCallable)
		void Interact();


protected: 

	virtual void BeginPlay() override;


private:

	/** Perform interaction on Server. */
	UFUNCTION(Reliable, Server, WithValidation)
		void ServerInteract();

	/** Perform interaction on Server, Validation. */
	bool ServerInteract_Validate();

	/** Perform interaction on Server, Implementation. */
	void ServerInteract_Implementation();
		
};
