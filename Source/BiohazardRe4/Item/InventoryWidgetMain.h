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

public:

	void SetItemData(const FBItemData& Data);
	void ClearItemData();

	UFUNCTION()
	void DropItem();
	UFUNCTION()
	void CloseCheck();

	UFUNCTION(BlueprintImplementableEvent)
	void WidgetOn();
	UFUNCTION(BlueprintImplementableEvent)
	void WidgetOff();

	UFUNCTION(BlueprintImplementableEvent)
	void DropWidgetOn();
	UFUNCTION(BlueprintImplementableEvent)
	void DropWidgetOff();

	UFUNCTION(BlueprintImplementableEvent)
	void AddItem(EItemCode ItemCode, int Num);

	UFUNCTION(BlueprintImplementableEvent)
	void AddItemRowName(FName ItemCode, int Num);

	UFUNCTION(BlueprintImplementableEvent)
	void SetMoney(int Num);

	UFUNCTION(BlueprintImplementableEvent)
	void AddMoney(int Num);

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	FString ItemName;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	FString ItemInformation;
};
