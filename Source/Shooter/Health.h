// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Health.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SHOOTER_API UHealth : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UHealth();

	//Network scaffolding
	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	/** Maximum amount of health */
	UPROPERTY(EditAnywhere)
		float maxHealth;

protected:

	// Called when the game starts
	virtual void BeginPlay() override;

	/** Remaining amount of Health */
	UPROPERTY(ReplicatedUsing = OnRep_CurrentHealth)
		float currentHealth;

	/** Handle when Health has been replicated*/
	UFUNCTION()
		virtual void OnRep_CurrentHealth();

	/** Die - Currently just destroys the object */
	void Die();

public:	

	/** Take damage by a certain amount */
	void Damage(float amount);

		
};
