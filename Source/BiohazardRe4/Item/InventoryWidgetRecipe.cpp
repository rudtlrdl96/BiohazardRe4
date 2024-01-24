// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryWidgetRecipe.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "InventoryWidgetCraft.h"
#include "InventoryManager.h"

void UBInventoryWidgetRecipe::NativeOnInitialized()
{
	Button = Cast<UButton>(GetWidgetFromName(TEXT("Button")));
	FScriptDelegate Delegate;
	Delegate.BindUFunction(this, TEXT("Create"));
	Button->OnClicked.Add(Delegate);
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

	if (Recipe.AItem == Recipe.BItem)
	{
		// ���� ������ 2���� ���� ������ ��� (�� + ��) ��꿡 ������ ���̽�
		if (UBInventoryManager::Instance->GetItemNum(Recipe.AItem) < Recipe.ANum + Recipe.BNum)
		{
			// A�䱸�� B�䱸���� ��ģ�ͺ��� �������� ���ٸ�
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

void UBInventoryWidgetRecipe::Create()
{
	UBInventoryManager::Instance->RemoveItem(CurRecipe.AItem, CurRecipe.ANum);
	UBInventoryManager::Instance->RemoveItem(CurRecipe.BItem, CurRecipe.BNum);
	UBInventoryManager::Instance->AddItem(CurRecipe.ResultItem);
}
