// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryWeapon.h"
#include "InventoryActor.h"
#include "InventoryManager.h"
int32 ABInventoryWeapon::GetStoredAmmo() const
{
	switch (GetItemCode())
	{
	case EItemCode::Handgun_SR09R:
		return ABInventoryActor::Instance->Inventory->GetItemNum(EItemCode::HandgunAmmo);
	case EItemCode::Shotgun_W870:
		return ABInventoryActor::Instance->Inventory->GetItemNum(EItemCode::ShotgunShells);
	case EItemCode::Rifle_SRM1903:
		return ABInventoryActor::Instance->Inventory->GetItemNum(EItemCode::RifleAmmo);
	default:
		return -1;
	}
}
