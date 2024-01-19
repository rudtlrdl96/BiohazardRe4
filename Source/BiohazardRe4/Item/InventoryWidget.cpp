// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryWidget.h"

void UBInventoryWidget::SetItemData(const FBItemData& Data)
{
	ItemName = Data.ItemName.ToString();
	ItemInformation = Data.ItemInformation;
}

inline void UBInventoryWidget::ClearItemData()
{
	ItemName = "";
	ItemInformation = "";
}
