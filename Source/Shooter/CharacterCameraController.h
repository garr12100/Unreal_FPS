// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Classes/Components/TimelineComponent.h"

#include "CharacterCameraController.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SHOOTER_API UCharacterCameraController : public UActorComponent
{
	GENERATED_BODY()

		class UTimelineComponent* zoomTimeline;

public:	
	// Sets default values for this component's properties
	UCharacterCameraController();

protected:
	virtual void BeginPlay() override;

	bool zoomedIn = false;
	float ZoomFOV;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;



	UPROPERTY(EditAnywhere, Category = "Zoom")
		float DefaultFOV = 90.f;

	UPROPERTY(EditAnywhere, Category = "Zoom")
		float DefaultZoomFOV = 80.f;

	UPROPERTY(EditAnywhere, Category = "Zoom")
		bool TickBased = false;

	UPROPERTY(EditAnywhere, Category = "Zoom")
		float TickSmoothing = 1.f;

	UPROPERTY(EditAnywhere, Category = "Zoom")
		class UCurveFloat* zoomCurve;

	UPROPERTY(EditAnywhere, Category = "Zoom")
		class UCameraComponent* Camera;

	/**Declare our delegate function to be bound to ZoomTimelineFloatReturn(float value) */
	FOnTimelineFloat ZoomInterpFunction {};

	/**Declare our delegate function to be bound to ZoomTimelineFinished */

	FOnTimelineEvent ZoomTimelineFinished {};

	UFUNCTION()
		void ZoomTimelineFloatReturn(float value);

	UFUNCTION()
		void OnZoomTimelineFinished();

	UFUNCTION()
		void Zoom(bool in, float FOV = -1.f);



};
