// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Player/Weapon/Gun/BLeonPistol.h"
#include "Item/InventoryActor.h"
#include "Item/InventoryWeapon.h"
#include "DamageType/PlayerDamageType/BDMGPlayerPistol.h"


ABLeonPistol::ABLeonPistol()
{
	AmmoType = EItemCode::HandgunAmmo;
	DefaultDamage = 1.f;
	RateOfFire = 1.f;
	DamageType = UBDMGPlayerPistol::StaticClass();
}



void ABLeonPistol::BeginPlay()
{
	Super::BeginPlay();
	if (InventoryInst == nullptr)
	{
		LOG_WARNING(TEXT("InventoryInst == nullptr"))
			return;
	}

	CurGun = InventoryInst->FindWeapon(EItemCode::Handgun_SR09R);
	SetCurLoopState();
	LOG_MSG(TEXT("ABLeonPistol::BeginPlay()"))
}