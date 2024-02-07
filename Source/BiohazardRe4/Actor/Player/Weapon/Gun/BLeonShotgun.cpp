// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Player/Weapon/Gun/BLeonShotgun.h"
#include "Item/InventoryActor.h"

ABLeonShotgun::ABLeonShotgun()
{
	if (InventoryInst == nullptr)
	{
		LOG_ERROR(TEXT("InventoryInst == nullptr"))
			return;
	}

	CurGun = InventoryInst->FindWeapon(EItemCode::Shotgun_W870);
	AmmoType = EItemCode::ShotgunShells;
	DefaultDamage = 5.6f;
	RateOfFire = 0.45f;
	Range = 2500.f;
}

void ABLeonShotgun::BeginPlay()
{
	Super::BeginPlay();
}
