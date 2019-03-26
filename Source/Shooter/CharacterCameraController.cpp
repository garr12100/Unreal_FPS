// Fill out your copyright notice in the Description page of Project Settings.

#include "CharacterCameraController.h"
#include "Classes/Components/TimelineComponent.h"
#include "Classes/Camera/CameraComponent.h"

// Sets default values for this component's properties
UCharacterCameraController::UCharacterCameraController()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	zoomTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("Zoom Timeline"));

	ZoomInterpFunction.BindUFunction(this, FName("ZoomTimelineFloatReturn"));
	ZoomTimelineFinished.BindUFunction(this, FName("OnZoomTimelineFinished"));


	// ...
}


// Called when the game starts
void UCharacterCameraController::BeginPlay()
{
	Super::BeginPlay();
	ZoomFOV = DefaultZoomFOV;
	// ...

	//Zoom in using timeline
	if (zoomCurve)
	{
		//Add curve to timeline and connect to delegates
		zoomTimeline->AddInterpFloat(zoomCurve, ZoomInterpFunction, FName("Alpha"));
		zoomTimeline->SetTimelineFinishedFunc(ZoomTimelineFinished);
		zoomTimeline->SetIgnoreTimeDilation(false);
	}

}


// Called every frame
void UCharacterCameraController::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!Camera || !TickBased)
		return;

	float targetFOV = zoomedIn ? ZoomFOV : DefaultFOV;

	Camera->FieldOfView = FMath::Lerp(Camera->FieldOfView, targetFOV, TickSmoothing * DeltaTime);
	// ...
}

void UCharacterCameraController::ZoomTimelineFloatReturn(float value)
{
	if (!Camera)
		return;
	Camera->FieldOfView = FMath::Lerp(DefaultFOV, ZoomFOV, value);
}

void UCharacterCameraController::OnZoomTimelineFinished()
{
	if (!zoomedIn)
		ZoomFOV = DefaultZoomFOV;
}

void UCharacterCameraController::Zoom(bool in, float FOV)
{
	zoomedIn = in;
	if (FOV > 0)
		ZoomFOV = FOV;
	if (!TickBased)
	{
		if (in)
			zoomTimeline->Play();
		else
			zoomTimeline->Reverse();
	}
}

