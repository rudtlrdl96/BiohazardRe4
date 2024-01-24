// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryWidgetRecipe.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "InventoryWidgetCraft.h"
#include "InventoryManager.h"

UBInventoryWidgetCraft* UBInventoryWidgetRecipe::CraftWidget = nullptr;

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
	ItemNum->SetText(FText::FromString(FString::FromInt(CraftWidget->Inventory->GetItemNum(Recipe.ResultItem))));
	ItemANum->SetText(FText::FromString(FString::FromInt(Recipe.ANum)));
	ItemBNum->SetText(FText::FromString(FString::FromInt(Recipe.BNum)));

}
