// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventoryWidgetButton.generated.h"

DECLARE_DELEGATE(ButtonEvent);

/**
 * 
 */
UCLASS()
class BIOHAZARDRE4_API UBInventoryWidgetButton : public UUserWidget
{
	GENERATED_BODY()

public:

	ButtonEvent OnClickedEvent;
	ButtonEvent OnHoveredEvent;
	ButtonEvent OnUnhoveredEvent;

	UFUNCTION(BlueprintCallable)
	void OnClicked();

	UFUNCTION(BlueprintCallable)
	void OnHovered();

	UFUNCTION(BlueprintCallable)
	void OnUnhovered();
};
