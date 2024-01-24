// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryWidgetMain.h"

void UBInventoryWidgetMain::SetItemData(const FBItemData& Data)
{
	ItemName = Data.ItemName.ToString();
	ItemInformation = Data.ItemInformation;
}

inline void UBInventoryWidgetMain::ClearItemData()
{
	ItemName = "";
	ItemInformation = "";
}
