// Fill out your copyright notice in the Description page of Project Settings.

#include "Health.h"
#include "GameFramework/Actor.h"
#include "Net/UnrealNetwork.h"


// Sets default values for this component's properties
UHealth::UHealth()
{
	PrimaryComponentTick.bCanEverTick = false;
	SetIsReplicated(true);
}

void UHealth::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UHealth, currentHealth);

}


// Called when the game starts
void UHealth::BeginPlay()
{
	Super::BeginPlay();
	currentHealth = maxHealth;	
}

void UHealth::OnRep_CurrentHealth()
{
	if (currentHealth <= 0)
		Die();
}

void UHealth::Die()
{
	GetOwner()->Destroy();
}

void UHealth::Damage(float amount)
{
	if (GetOwner()->Role == ROLE_Authority)
	{
		currentHealth -= amount;
		if (currentHealth <= 0)
			Die();
	}
}

