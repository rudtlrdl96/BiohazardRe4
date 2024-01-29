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
	void SetParentUI(class ABMapUIActor* Parent);

	void SetCurrentFloor(EFloor Floor)
	{
		CurrentFloor = Floor;
	}

	void SetPrevFloor(EFloor Floor)
	{
		PrevFloor = Floor;
	}

	UFUNCTION(BlueprintImplementableEvent)
	void ChangeMapLayerWidget();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EFloor PrevFloor = EFloor::E_1F;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EFloor CurrentFloor = EFloor::E_1F;

private:
	UPROPERTY()
	class ABMapUIActor* ParentMapUI = nullptr;


};
