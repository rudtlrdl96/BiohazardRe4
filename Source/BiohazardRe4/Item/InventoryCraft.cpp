// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryCraft.h"
#include "Components/CanvasPanel.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Components/Image.h"

void UBInventoryCraft::NativeOnInitialized()
{
	MainPanel = Cast<UCanvasPanel>(GetWidgetFromName(TEXT("MainPanel")));
	SelectItemName = Cast<UTextBlock>(GetWidgetFromName(TEXT("ItemName")));

	Panels.SetNum(3);
	Panels[0] = Cast<UCanvasPanel>(GetWidgetFromName(TEXT("CanvasPanel_0")));
	Panels[1] = Cast<UCanvasPanel>(GetWidgetFromName(TEXT("CanvasPanel_1")));
	Panels[2] = Cast<UCanvasPanel>(GetWidgetFromName(TEXT("CanvasPanel_2")));

	Buttons.SetNum(3);
	Buttons[0] = Cast<UButton>(GetWidgetFromName(TEXT("Button_0")));
	Buttons[1] = Cast<UButton>(GetWidgetFromName(TEXT("Button_1")));
	Buttons[2] = Cast<UButton>(GetWidgetFromName(TEXT("Button_2")));

	ItemImages.SetNum(3);
	ItemImages[0] = Cast<UImage>(GetWidgetFromName(TEXT("Image_0")));
	ItemImages[1] = Cast<UImage>(GetWidgetFromName(TEXT("Image_1")));
	ItemImages[2] = Cast<UImage>(GetWidgetFromName(TEXT("Image_2")));

	ItemNames.SetNum(3);
	ItemNames[0] = Cast<UTextBlock>(GetWidgetFromName(TEXT("Text_0")));
	ItemNames[1] = Cast<UTextBlock>(GetWidgetFromName(TEXT("Text_1")));
	ItemNames[2] = Cast<UTextBlock>(GetWidgetFromName(TEXT("Text_2")));

	IngredientsAImages.SetNum(3);
	IngredientsAImages[0] = Cast<UImage>(GetWidgetFromName(TEXT("ingredientsA_0")));
	IngredientsAImages[1] = Cast<UImage>(GetWidgetFromName(TEXT("ingredientsA_1")));
	IngredientsAImages[2] = Cast<UImage>(GetWidgetFromName(TEXT("ingredientsA_2")));

	IngredientsBImages.SetNum(3);
	IngredientsBImages[0] = Cast<UImage>(GetWidgetFromName(TEXT("ingredientsB_0")));
	IngredientsBImages[1] = Cast<UImage>(GetWidgetFromName(TEXT("ingredientsB_1")));
	IngredientsBImages[2] = Cast<UImage>(GetWidgetFromName(TEXT("ingredientsB_2")));

	IngredientsANum.SetNum(3);
	IngredientsANum[0] = Cast<UTextBlock>(GetWidgetFromName(TEXT("Text_0")));
	IngredientsANum[1] = Cast<UTextBlock>(GetWidgetFromName(TEXT("Text_1")));
	IngredientsANum[2] = Cast<UTextBlock>(GetWidgetFromName(TEXT("Text_2")));
}

void UBInventoryCraft::SetHide()
{
}

void UBInventoryCraft::SetItemData(const FBItemData& Item)
{
	switch (Item.ItemCode)
	{
	case EItemCode::GreenHerb:
	{
		Panels[1]->SetVisibility(ESlateVisibility::Visible);
		Panels[2]->SetVisibility(ESlateVisibility::Visible);
		break;
	}
	case EItemCode::RedHerb:
		Panels[1]->SetVisibility(ESlateVisibility::Hidden);
		Panels[2]->SetVisibility(ESlateVisibility::Hidden);
		break;
	case EItemCode::Gunpowder:
		Panels[1]->SetVisibility(ESlateVisibility::Visible);
		Panels[2]->SetVisibility(ESlateVisibility::Visible);
		break;
	case EItemCode::Resources_S:
		Panels[1]->SetVisibility(ESlateVisibility::Visible);
		Panels[2]->SetVisibility(ESlateVisibility::Hidden);
		break;
	case EItemCode::Resources_L:
		Panels[1]->SetVisibility(ESlateVisibility::Hidden);
		Panels[2]->SetVisibility(ESlateVisibility::Hidden);
		break;
	default:
		break;
	}
}
