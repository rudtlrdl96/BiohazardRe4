// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryWidgetCraft.h"
#include "Components/CanvasPanel.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/ScrollBox.h"
#include "InventoryWidgetRecipe.h"
#include "CraftRecipe.h"
#include "BiohazardRe4.h"
void UBInventoryWidgetCraft::NativeOnInitialized()
{
	MainPanel = Cast<UCanvasPanel>(GetWidgetFromName(TEXT("MainPanel")));
	SelectItemName = Cast<UTextBlock>(GetWidgetFromName(TEXT("ItemName")));
	ScrollBox = Cast<UScrollBox>(GetWidgetFromName(TEXT("Scroll")));
	UBInventoryWidgetRecipe::CraftWidget = this;
}

void UBInventoryWidgetCraft::SetHide()
{
}

void UBInventoryWidgetCraft::SetItemData(const FBItemData& Item)
{
	if (!RecipeTable)
	{
		LOG_ERROR(TEXT("DataTable Not Setting in WidgetBlueprint"))
		return;
	}

	for (UBInventoryWidgetRecipe* Widget : RecipeWidgets)
	{
		Widget->RemoveFromParent();
	}
	RecipeWidgets.SetNum(0);

	SelectItemName->SetText(FText::FromName(Item.ItemName));

	TArray<FName> Names = RecipeTable->GetRowNames();
	for (FName Name : Names)
	{
		FBCraftRecipe* Recipe = RecipeTable->FindRow<FBCraftRecipe>(Name, TEXT("Recipe Can not Find"));
		if (Item.ItemCode == Recipe->AItem || Item.ItemCode == Recipe->BItem)
		{
			UBInventoryWidgetRecipe* Widget = CreateWidget<UBInventoryWidgetRecipe>(this, RecipeWidgetClass);
			Widget->SetRecipe(*Recipe);
			ScrollBox->AddChild(Widget);
			RecipeWidgets.Add(Widget);
		}
	}
}
