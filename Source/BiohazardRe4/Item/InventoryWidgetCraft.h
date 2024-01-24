// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ItemData.h"
#include "InventoryWidgetCraft.generated.h"

/**
 * 
 */
class UCanvasPanel;
class UTextBlock;
class UScrollBox;

UCLASS()
class BIOHAZARDRE4_API UBInventoryWidgetCraft : public UUserWidget
{
	GENERATED_BODY()

	virtual void NativeOnInitialized() override;

	UPROPERTY()
	UCanvasPanel* MainPanel;		// ���� �ֻ��� Canvas
	UPROPERTY()
	UTextBlock* SelectItemName;		// ���� ������ �������� �̸�
	UPROPERTY()
	UScrollBox* ScrollBox;
public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UDataTable* RecipeTable;

	UPROPERTY(EditAnywhere, Category = "Widget")
	TSubclassOf<class UBInventoryWidgetRecipe> RecipeWidgetClass;	// ������ �ൿ ���� Ŭ����

	TArray<UBInventoryWidgetRecipe*> RecipeWidgets;

	void SetItemData(const FBItemData& Item);
};
