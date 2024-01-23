// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ItemData.h"
#include "InventoryCraft.generated.h"

/**
 * 
 */
class UCanvasPanel;
class UTextBlock;
class UButton;
class UImage;

UCLASS()
class BIOHAZARDRE4_API UBInventoryCraft : public UUserWidget
{
	GENERATED_BODY()

	virtual void NativeOnInitialized() override;

	UPROPERTY()
	UCanvasPanel* MainPanel;		// ���� �ֻ��� Canvas
	UPROPERTY()
	UTextBlock* SelectItemName;		// ���� ������ �������� �̸�
	UPROPERTY()
	TArray<UCanvasPanel*> Panels;	// �� ��ư�� Canvas
	UPROPERTY()
	TArray<UButton*> Buttons;		// ���� ��ư
	UPROPERTY()
	TArray<UImage*> ItemImages;		// ��� �������� �̹���
	UPROPERTY()
	TArray<UTextBlock*> ItemNames;	// ��� �������� �̸�
	UPROPERTY()
	TArray<UImage*> IngredientsAImages;		// ��� A�� �̹���
	UPROPERTY()
	TArray<UTextBlock*> IngredientsANum;	// ��� A�� ����
	UPROPERTY()
	TArray<UImage*> IngredientsBImages;		// ��� B�� �̹���
	UPROPERTY()
	TArray<UTextBlock*> IngredientsBNum;	// ��� B�� ����

public:
	UPROPERTY()
	class UBInventoryManager* Inventory;

	void SetHide();

	void SetItemData(const FBItemData& Item);
};
