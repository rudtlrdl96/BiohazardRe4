// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Player/Weapon/Gun/BLeonRifle.h"
#include "Item/InventoryActor.h"

ABLeonRifle::ABLeonRifle()
{
	if (InventoryInst == nullptr)
	{
		LOG_WARNING(TEXT("InventoryInst == nullptr"))
			return;
	}

	CurGun = InventoryInst->FindWeapon(EItemCode::Rifle_SRM1903);
	AmmoType = EItemCode::RifleAmmo;
	DamageUnit = 100.f;
	DefaultDamage = 2.5f;
	RateOfFire = 0.42f;
	Range = 10000.f;
}

void ABLeonRifle::BeginPlay()
{
	Super::BeginPlay();
}
