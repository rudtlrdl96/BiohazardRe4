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
	UCanvasPanel* MainPanel;		// 가장 최상위 Canvas
	UPROPERTY()
	UTextBlock* SelectItemName;		// 현재 선택한 아이템의 이름
	UPROPERTY()
	TArray<UCanvasPanel*> Panels;	// 각 버튼별 Canvas
	UPROPERTY()
	TArray<UButton*> Buttons;		// 조합 버튼
	UPROPERTY()
	TArray<UImage*> ItemImages;		// 결과 아이템의 이미지
	UPROPERTY()
	TArray<UTextBlock*> ItemNames;	// 결과 아이템의 이름
	UPROPERTY()
	TArray<UImage*> IngredientsAImages;		// 재료 A의 이미지
	UPROPERTY()
	TArray<UTextBlock*> IngredientsANum;	// 재료 A의 개수
	UPROPERTY()
	TArray<UImage*> IngredientsBImages;		// 재료 B의 이미지
	UPROPERTY()
	TArray<UTextBlock*> IngredientsBNum;	// 재료 B의 개수

public:
	UPROPERTY()
	class UBInventoryManager* Inventory;

	void SetHide();

	void SetItemData(const FBItemData& Item);
};
