// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/InventoryWidgetButton.h"

void UBInventoryWidgetButton::OnClicked()
{
	OnClickedEvent.ExecuteIfBound();
}

void UBInventoryWidgetButton::OnHovered()
{
	OnHoveredEvent.ExecuteIfBound();
}

void UBInventoryWidgetButton::OnUnhovered()
{
	OnHoveredEvent.ExecuteIfBound();
}
