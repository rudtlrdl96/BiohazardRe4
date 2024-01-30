// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventoryWidgetQuickSlot.generated.h"
/**
 * 
 */
class UButton;
class UImage;

UCLASS()
class BIOHAZARDRE4_API UBInventoryWidgetQuickSlot : public UUserWidget
{
	GENERATED_BODY()
	
	virtual void NativeOnInitialized() override;

public :

	UPROPERTY(BlueprintReadOnly)
	TArray<class ABInventoryWeapon*> QuickSlot;

	UPROPERTY(BlueprintReadOnly)
	ABInventoryWeapon* AddWeaponPtr;

	UFUNCTION(BlueprintCallable)
	void CloseQuickSlot();

	UFUNCTION(BlueprintImplementableEvent)
	void WidgetOn();
	UFUNCTION(BlueprintImplementableEvent)
	void WidgetOff();
};
