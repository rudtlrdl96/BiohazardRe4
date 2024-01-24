// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CraftRecipe.h"
#include "InventoryWidgetRecipe.generated.h"

/**
 * 
 */
class UButton;
class UImage;
class UTextBlock;
class UBInventoryWidgetCraft;

UCLASS()
class BIOHAZARDRE4_API UBInventoryWidgetRecipe : public UUserWidget
{
	GENERATED_BODY()

	friend UBInventoryWidgetCraft;

	static UBInventoryWidgetCraft* CraftWidget;

	virtual void NativeOnInitialized() override;

	UButton* Button;		// 조합 버튼
	UPROPERTY()
	UImage* ItemImage;		// 결과 아이템의 이미지
	UPROPERTY()
	UTextBlock* ItemName;	// 결과 아이템의 이름
	UPROPERTY()
	UTextBlock* ItemNum;	// 결과 아이템의 현재 개수
	UPROPERTY()
	UImage* ItemAImage;		// 재료 A의 이미지
	UPROPERTY()
	UTextBlock* ItemANum;	// 재료 A의 개수
	UPROPERTY()
	UImage* ItemBImage;		// 재료 B의 이미지
	UPROPERTY()
	UTextBlock* ItemBNum;	// 재료 B의 개수

public:
	void SetRecipe(const FBCraftRecipe& Recipe);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<EItemCode, UObject*> ItemIconMap;

};
