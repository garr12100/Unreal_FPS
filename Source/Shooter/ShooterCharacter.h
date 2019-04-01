// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ShooterCharacter.generated.h"

class UInputComponent;

UCLASS(config=Game)
class AShooterCharacter : public ACharacter
{
	GENERATED_BODY()

private:

	/** Pawn mesh: 1st person view (arms; seen only by self) */
	UPROPERTY(VisibleDefaultsOnly, Category=Mesh)
	class USkeletalMeshComponent* Mesh1P;

	/** Pawn mesh: 3rd person view (body; seen only by others) */
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
		class USkeletalMeshComponent* Mesh3P;

	/** First person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FirstPersonCameraComponent;

	float defaultSpeed;

	UFUNCTION()
		void HandleDestroyed(AActor* actor);

	void SetDefaultGun();


protected:

	virtual void BeginPlay();

	UPROPERTY(EditAnywhere, Category = Gameplay, ReplicatedUsing = OnRep_Gun)
		class AGun* gun;

	/** AnimMontage to play each time we fire */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay, Replicated)
		bool Focused = false;

	/** AnimMontage to play each time we fire */
	UPROPERTY(BlueprintReadOnly, Category = Gameplay)
		bool Paused = false;

	/** Begin Firing */
	void OnFire();

	/** Stop Firing */
	void OnStopFire();

	/** Handles moving forward/backward */
	void MoveForward(float Val);

	/** Handles stafing movement, left and right */
	void MoveRight(float Val);

	/**
	 * Called via input to turn at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);

	/** APawn interface */
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;

	/** Drop Gun into world */
	void DetachGun();

	/** Attach gun to socket */
	void AttachGun();

	/** Tell Interactor Component to perform an interaction */
	void Interact();

	/** Focus by zooming in and adjusting movement speed */
	void Focus();

	/** Undo the Focus */
	void Unfocus();

	/** Handle when the Gun has been replicated*/
	UFUNCTION()
		virtual void OnRep_Gun();

	/**Server-side functions for focusing */
	UFUNCTION(Server, Reliable, WithValidation)
		void ServerFocus(bool _focused);
	virtual void ServerFocus_Implementation(bool _focused);
	virtual bool ServerFocus_Validate(bool _focused);

	/** Set Player Spectate */
	void SetPlayerSpectate();

	/**Client-side functions for Spectate/Play */
	UFUNCTION(NetMulticast, Reliable)
		void MulticastSetPlayerSpectate();
	virtual void MulticastSetPlayerSpectate_Implementation();

public:
	AShooterCharacter();

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;

	/** Component for Interactor - allows interaction with game objects */
	UPROPERTY(VisibleDefaultsOnly, Category = Gameplay)
		class UInteractor* Interactor;

	/** Component for Health - allows player to take damage */
	UPROPERTY(VisibleDefaultsOnly, Category = Gameplay)
		class UHealth* Health;

	/** First Person AnimMontage to play each time we fire */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		class UAnimMontage* FireAnimation1P;

	/** Third Person AnimMontage to play each time we fire */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		class UAnimMontage* FireAnimation3P;

	/** Multiplier to movement speed that is applied when player Focuses (zooms in) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		float FocusedSpeedMultiplier = .5f;

	/** Camera component */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera)
		class UCharacterCameraController* CameraControllerComponent;

	/**Reference to AGun-derived blueprint that can spawn as default gun */
	UPROPERTY(EditAnywhere, Category = Gameplay)
		TSubclassOf<class AGun> defaultGun_BP;

	/**Is this character paused? Will prevent input from working. */
	UFUNCTION(BlueprintCallable)
		void SetPaused(bool on);

	/**Is this character paused? Will prevent input from working. */
	UFUNCTION(BlueprintCallable)
		class USkeletalMeshComponent* GetMesh3P() const;

	/** Getter for Camera Component */
	FORCEINLINE class UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }

	virtual FVector GetPawnViewLocation() const override;

	/** Trigger fire animation based on 1st or 3rd person model */
	void PlayFireAnimation();

	/** Equip a new gun */
	void SetGun(class AGun* newGun);

	/** Equip a new gun by instantiating it from a blueprint */
	void SetGunFromBlueprint(TSubclassOf<class AGun> newGunBP);

	/** Getter for Focused */
	bool IsFocused() const;

	/** Set Player to Spectator */
	void SetStateSpectator();

	/** Set Player to Playing */
	void SetStatePlayer();

};

