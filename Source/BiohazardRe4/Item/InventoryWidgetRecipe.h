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

	UButton* Button;		// ���� ��ư
	UPROPERTY()
	UImage* ItemImage;		// ��� �������� �̹���
	UPROPERTY()
	UTextBlock* ItemName;	// ��� �������� �̸�
	UPROPERTY()
	UTextBlock* ItemNum;	// ��� �������� ���� ����
	UPROPERTY()
	UImage* ItemAImage;		// ��� A�� �̹���
	UPROPERTY()
	UTextBlock* ItemANum;	// ��� A�� ����
	UPROPERTY()
	UImage* ItemBImage;		// ��� B�� �̹���
	UPROPERTY()
	UTextBlock* ItemBNum;	// ��� B�� ����

public:
	void SetRecipe(const FBCraftRecipe& Recipe);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<EItemCode, UObject*> ItemIconMap;

};
