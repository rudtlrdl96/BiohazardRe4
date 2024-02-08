// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Player/Weapon/Gun/BLeonPistol.h"
#include "Item/InventoryActor.h"
#include "Item/InventoryWeapon.h"


ABLeonPistol::ABLeonPistol()
{

}

void ABLeonPistol::SpawnShell_Implementation()
{
	LOG_MSG(TEXT("SpawnShell"))
}

void ABLeonPistol::SpawnMagazine_Implementation()
{
	LOG_MSG(TEXT("SpawnMagazine"))
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
	AmmoType = EItemCode::HandgunAmmo;
	DefaultDamage = 1.f;
	RateOfFire = 1.f;
	SetCurLoopState();
	LOG_MSG(TEXT("ABLeonPistol::BeginPlay()"))
}

void ABLeonPistol::DropMagazine()
{
	Super::DropMagazine();
	SpawnMagazine();
	LOG_MSG(TEXT("ABLeonPistol::DropMagazine"))
}

void ABLeonPistol::DropShell()
{
	Super::DropShell();
	SpawnShell();
	LOG_MSG(TEXT("ABLeonPistol::DropShell"))
}
