// Fill out your copyright notice in the Description page of Project Settings.


#include "HUDBase.h"
#include "Item/InventoryActor.h"

ABHUDBase* ABHUDBase::Instance = nullptr;

ABHUDBase::ABHUDBase()
{
	Instance = this;
}

ABInventoryWeapon* ABHUDBase::GetWeapon(EItemCode Code)
{
	return ABInventoryActor::Instance->FindWeapon(Code);
}
