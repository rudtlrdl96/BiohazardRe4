// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Player/Weapon/Gun/BLeonShotgun.h"
#include "Item/InventoryActor.h"
#include "Item/InventoryWeapon.h"

ABLeonShotgun::ABLeonShotgun()
{
	AmmoType = EItemCode::ShotgunShells;
	DefaultDamage = 5.6f;
	RateOfFire = 0.45f;
	Range = 2500.f;
}

void ABLeonShotgun::BeginPlay()
{
	Super::BeginPlay();
	if (InventoryInst == nullptr)
	{
		LOG_WARNING(TEXT("InventoryInst == nullptr"))
			return;
	}

	CurGun = InventoryInst->FindWeapon(EItemCode::Shotgun_W870);
	SetCurLoopState();
}

void ABLeonShotgun::Reload()
{
	if (InventoryInst == nullptr)
	{
		LOG_WARNING(TEXT("InventoryInst == nullptr"))
			return;
	}

	if (CurGun == nullptr)
	{
		LOG_WARNING(TEXT("CurGun == nullptr"))
			return;
	}

	int32 MaxAmmo = CurGun->GetMaxAmmo();
	int32 CurAmmo = CurGun->GetLoadedAmmo();
	int32 StoredAmmo = InventoryInst->GetItemCount(AmmoType);
	int32 ReloadAmmoCount = MaxAmmo - CurAmmo;

	if (ReloadAmmoCount > StoredAmmo)
	{
		ReloadAmmoCount = StoredAmmo;
	}

	CurAmmo += 1;
	CurGun->SetLoadedAmmo(CurAmmo);
	InventoryInst->RemoveItem(AmmoType);
	LOG_MSG(TEXT("ABLeonRifle::Reload"))

		if (CurAmmo == MaxAmmo)
		{
			ReloadEnd();
		}
		else
		{
			LOG_MSG(TEXT("ABLeonRifle::Reload_Loop"))
			CurState = EGunState::EReload_Loop;
		}
}

void ABLeonShotgun::ReloadEnd()
{
	LOG_MSG(TEXT("ABLeonRifle::ReloadEnd"))
		CurState = EGunState::EReload_End;
}

void ABLeonShotgun::ReloadEndAfter()
{
	LOG_MSG(TEXT("ABLeonRifle::ReloadEndAfter"))
		CurState = EGunState::EIdle_Loop;
}