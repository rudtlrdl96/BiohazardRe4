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

	bool bFlag = false;
	if (Recipe.AItem == Recipe.BItem)
	{
		int ItemCount = UBInventoryManager::Instance->GetItemNum(Recipe.AItem);
		// 조합 아이템 2개가 같은 종류인 경우 (녹 + 녹) 허브에 한정된 케이스
		if (Recipe.ANum <= ItemCount)
		{
			// A요구량을 넘겼다면
			ItemANum->SetColorAndOpacity(ActiveTextColor);
			if (Recipe.ANum + Recipe.BNum <= ItemCount)
			{
				// A 요구량과 B 요구량을 포함해 넘겼다면
				ItemBNum->SetColorAndOpacity(ActiveTextColor);
			}
			else
			{
				// B 요구량을 넘기지 못함
				ItemBNum->SetColorAndOpacity(InactiveTextColor);
				bFlag = true;
			}
		}
		else
		{
			ItemANum->SetColorAndOpacity(InactiveTextColor);
			ItemBNum->SetColorAndOpacity(InactiveTextColor);
			bFlag = true;
		}
	}
	else
	{
		if (Recipe.ANum <= UBInventoryManager::Instance->GetItemNum(Recipe.AItem))
		{
			// A요구량을 넘김
			ItemANum->SetColorAndOpacity(ActiveTextColor);
		}
		else
		{
			ItemANum->SetColorAndOpacity(InactiveTextColor);
			bFlag = true;
		}

		if (Recipe.BNum <= UBInventoryManager::Instance->GetItemNum(Recipe.BItem))
		{
			// B 요구량을 넘김
			ItemBNum->SetColorAndOpacity(ActiveTextColor);
		}
		else
		{
			ItemBNum->SetColorAndOpacity(InactiveTextColor);
			bFlag = true;
		}
	}

	if (bFlag)
	{
		Button->SetIsEnabled(false);
	}
	else
	{
		CurRecipe = Recipe;
		Button->SetIsEnabled(true);
	}
}

void UBInventoryWidgetRecipe::Craft()
{
	UBInventoryManager::Instance->CraftItem(CurRecipe);
	ABInventoryActor::Instance->CompleteCraft();
}
