// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryWidgetRecipe.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "InventoryWidgetCraft.h"
#include "InventoryManager.h"
#include "InventoryActor.h"

void UBInventoryWidgetRecipe::NativeOnInitialized()
{
	Button = Cast<UButton>(GetWidgetFromName(TEXT("Button")));
	ItemImage = Cast<UImage>(GetWidgetFromName(TEXT("Image")));
	ItemName = Cast<UTextBlock>(GetWidgetFromName(TEXT("Text")));
	ItemNum = Cast<UTextBlock>(GetWidgetFromName(TEXT("ItemNum")));
	ItemAImage = Cast<UImage>(GetWidgetFromName(TEXT("ItemAImage")));
	ItemANum = Cast<UTextBlock>(GetWidgetFromName(TEXT("ItemANum")));
	ItemBImage = Cast<UImage>(GetWidgetFromName(TEXT("ItemBImage")));
	ItemBNum = Cast<UTextBlock>(GetWidgetFromName(TEXT("ItemBNum")));
}

void UBInventoryWidgetRecipe::SetRecipe(const FBCraftRecipe& Recipe)
{
	// 이미지
	ItemImage->SetBrushResourceObject(ItemIconMap[Recipe.ResultItem]);
	ItemAImage->SetBrushResourceObject(ItemIconMap[Recipe.AItem]);
	ItemBImage->SetBrushResourceObject(ItemIconMap[Recipe.BItem]);
	// 이름
	ItemName->SetText(FText::FromName(Recipe.ResultName));
	// 개수
	ItemNum->SetText(FText::FromString(FString::FromInt(UBInventoryManager::Instance->GetItemNum(Recipe.ResultItem))));
	ItemANum->SetText(FText::FromString(FString::FromInt(Recipe.ANum)));
	ItemBNum->SetText(FText::FromString(FString::FromInt(Recipe.BNum)));

	if (Recipe.AItem == Recipe.BItem)
	{
		// 조합 아이템 2개가 같은 종류인 경우 (녹 + 녹) 허브에 한정된 케이스
		if (UBInventoryManager::Instance->GetItemNum(Recipe.AItem) < Recipe.ANum + Recipe.BNum)
		{
			// A요구량 B요구량을 합친것보다 소지수가 적다면
			Button->SetIsEnabled(false);
			return;
		}
	}
	else
	{
		if (UBInventoryManager::Instance->GetItemNum(Recipe.AItem) < Recipe.ANum)
		{
			Button->SetIsEnabled(false);
			return;
		}
		if (UBInventoryManager::Instance->GetItemNum(Recipe.BItem) < Recipe.BNum)
		{
			Button->SetIsEnabled(false);
			return;
		}
	}
	CurRecipe = Recipe;
	Button->SetIsEnabled(true);
}

void UBInventoryWidgetRecipe::Craft()
{
	UBInventoryManager::Instance->CraftItem(CurRecipe);
	ABInventoryActor::Instance->CompleteCraft();
}
