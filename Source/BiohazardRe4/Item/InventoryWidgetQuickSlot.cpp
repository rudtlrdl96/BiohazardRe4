// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryWidgetQuickSlot.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "InventoryActor.h"
void UBInventoryWidgetQuickSlot::NativeOnInitialized()
{
	QuickSlot = ABInventoryActor::Instance->QuickSlot;
}

void UBInventoryWidgetQuickSlot::CloseQuickSlot()
{
	ABInventoryActor::Instance->CloseQuickSlot();
}
