// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventoryBehavior.generated.h"

/**
 * 
 */
UCLASS()
class BIOHAZARDRE4_API UBInventoryBehavior : public UUserWidget
{
	GENERATED_BODY()
	
	virtual void NativeOnInitialized() override;

	UPROPERTY()
	class UButton* Button;

public:
	void SetButton();
};
