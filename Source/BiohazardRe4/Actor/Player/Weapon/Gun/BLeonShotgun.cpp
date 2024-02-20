// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Player/Weapon/Gun/BLeonShotgun.h"
#include "Item/InventoryActor.h"
#include "Item/InventoryWeapon.h"
#include "Actor/Player/HUD/HUDBase.h"
#include "DamageType/PlayerDamageType/BDMGPlayerShotgun.h"

ABLeonShotgun::ABLeonShotgun()
{
	AmmoType = EItemCode::ShotgunShells;
	DefaultDamage = 5.6f;
	RateOfFire = 0.45f;
	Range = 500.f;
	DamageType = UBDMGPlayerShotgun::StaticClass();
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
	ABHUDBase::Instance->UpdateLoadedAmmo();
	ABHUDBase::Instance->UpdateStoredAmmo();

	LOG_MSG(TEXT("ABLeonShotgun::Reload"))

	if (CurAmmo == MaxAmmo)
	{
		ReloadEnd();
	}
}

void ABLeonShotgun::ReloadLoopStart()
{
	LOG_MSG(TEXT("ABLeonShotgun::ReloadLoopStart"))
		CurState = EGunState::EReload_Loop;
}

void ABLeonShotgun::ReloadEnd()
{
	LOG_MSG(TEXT("ABLeonShotgun::ReloadEnd"))
		CurState = EGunState::EReload_End;
}

void ABLeonShotgun::ReloadEndAfter()
{
	LOG_MSG(TEXT("ABLeonShotgun::ReloadEndAfter"))
		CurState = EGunState::EIdle_Loop;
}