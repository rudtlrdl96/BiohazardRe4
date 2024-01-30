// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemData.h"
#include "Blueprint/UserWidget.h"
#include "InventoryWidgetMain.generated.h"
/**
 * 
 */
class UCanvasPanel;
class UTextBlock;
class UButton;

UCLASS()
class BIOHAZARDRE4_API UBInventoryWidgetMain : public UUserWidget
{
	GENERATED_BODY()
	
	virtual void NativeOnInitialized() override;

	UPROPERTY()
	UCanvasPanel* WarningPanel;

	UPROPERTY()
	UTextBlock* WarningText;

	UPROPERTY()
	UButton* OkButton;

	UPROPERTY()
	UButton* NoButton;

	UFUNCTION()
	void Drop();

public:

	void SetItemData(const FBItemData& Data);
	inline void ClearItemData();

	UFUNCTION()
	void OnDropItem();
	UFUNCTION()
	void OffDropItem();
	UFUNCTION()
	void OffCloseCheck();

	UFUNCTION()
	void CloseCheck();

	UFUNCTION(BlueprintImplementableEvent)
	void WidgetOn();
	UFUNCTION(BlueprintImplementableEvent)
	void WidgetOff();

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	FString ItemName;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	FString ItemInformation;
};
