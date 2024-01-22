// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ItemData.h"
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
	TArray<class UCanvasPanel*> Panels;
	UPROPERTY()
	TArray<class UButton*> Buttons;
	UPROPERTY()
	TArray<class UImage*> Images;

	UPROPERTY()
	TArray<class UTextBlock*> Texts;

	UPROPERTY(EditAnywhere)
	TArray<UObject*> Icons;

public:
	void SetItemData(const FBItemData& Data);
};
