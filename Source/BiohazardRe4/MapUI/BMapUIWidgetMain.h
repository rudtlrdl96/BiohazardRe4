// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BMapUIActor.h"
#include "Blueprint/UserWidget.h"
#include "BMapUIWidgetMain.generated.h"

/**
 * 
 */
UCLASS()
class BIOHAZARDRE4_API UBMapUIWidgetMain : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void SetCurrentFloor(EFloor Floor)
	{
		CurrentFloor = Floor;
	}

	void SetPrevFloor(EFloor Floor)
	{
		PrevFloor = Floor;
	}

	void SetCameraFOV(float CurFOV)
	{
		CameraFOV = CurFOV;
	}

	void SetRangeOfFOV(float Min, float Max)
	{
		MinFOV = Min;
		MaxFOV = Max;
	}

	UFUNCTION(BlueprintImplementableEvent)
	void UpdateMapLayerWidget();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EFloor PrevFloor = EFloor::E_1F;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EFloor CurrentFloor = EFloor::E_1F;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float CameraFOV = 45.f;

	UPROPERTY(BlueprintReadOnly)
	float MaxFOV = 75.f;
	UPROPERTY(BlueprintReadOnly)
	float MinFOV = 25.f;
	
private:



};
