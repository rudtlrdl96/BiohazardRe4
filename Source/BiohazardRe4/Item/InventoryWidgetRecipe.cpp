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
	// �̹���
	ItemImage->SetBrushResourceObject(ItemIconMap[Recipe.ResultItem]);
	ItemAImage->SetBrushResourceObject(ItemIconMap[Recipe.AItem]);
	ItemBImage->SetBrushResourceObject(ItemIconMap[Recipe.BItem]);
	// �̸�
	ItemName->SetText(FText::FromName(Recipe.ResultName));
	// ����
	ItemNum->SetText(FText::FromString(FString::FromInt(UBInventoryManager::Instance->GetItemNum(Recipe.ResultItem))));
	ItemANum->SetText(FText::FromString(FString::FromInt(Recipe.ANum)));
	ItemBNum->SetText(FText::FromString(FString::FromInt(Recipe.BNum)));

	bool bFlag = false;
	if (Recipe.AItem == Recipe.BItem)
	{
		int ItemCount = UBInventoryManager::Instance->GetItemNum(Recipe.AItem);
		// ���� ������ 2���� ���� ������ ��� (�� + ��) ��꿡 ������ ���̽�
		if (Recipe.ANum <= ItemCount)
		{
			// A�䱸���� �Ѱ�ٸ�
			ItemANum->SetColorAndOpacity(ActiveTextColor);
			if (Recipe.ANum + Recipe.BNum <= ItemCount)
			{
				// A �䱸���� B �䱸���� ������ �Ѱ�ٸ�
				ItemBNum->SetColorAndOpacity(ActiveTextColor);
			}
			else
			{
				// B �䱸���� �ѱ��� ����
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
			// A�䱸���� �ѱ�
			ItemANum->SetColorAndOpacity(ActiveTextColor);
		}
		else
		{
			ItemANum->SetColorAndOpacity(InactiveTextColor);
			bFlag = true;
		}

		if (Recipe.BNum <= UBInventoryManager::Instance->GetItemNum(Recipe.BItem))
		{
			// B �䱸���� �ѱ�
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
