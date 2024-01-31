// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryWidgetQuickSlot.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "InventoryActor.h"

TArray<ABInventoryWeapon*>& UBInventoryWidgetQuickSlot::GetQuickSlot() const
{
	return ABInventoryActor::Instance->QuickSlot;
}

void UBInventoryWidgetQuickSlot::SetQuickSlotItem(ABInventoryWeapon* Weapon, int Index)
{
	TArray<ABInventoryWeapon*>& QuickSlot = ABInventoryActor::Instance->QuickSlot;
	for (int i = 0; i < QuickSlot.Num(); i++)
	{
		if (QuickSlot[i] == Weapon)
		{
			QuickSlot[i] = nullptr;
			break;
		}
	}
	QuickSlot[Index] = Weapon;
}

void UBInventoryWidgetQuickSlot::CloseQuickSlot()
{
	ABInventoryActor::Instance->CloseQuickSlot();
}
