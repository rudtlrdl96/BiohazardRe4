// Fill out your copyright notice in the Description page of Project Settings.

#include "Actor/Player/Weapon/Gun/BLeonGun.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/DamageEvents.h"
#include "Item/InventoryActor.h"
#include "Item/InventoryWeapon.h"
#include "DamageType/BDMGPlayerDamage.h"


ABLeonGun::ABLeonGun()
{

}

void ABLeonGun::BeginPlay()
{
	Super::BeginPlay();
	LOG_MSG(TEXT("ABLeonGun Constructor"))
	PlayerCamManager = UGameplayStatics::GetPlayerCameraManager(this, 0);
}


bool ABLeonGun::AbleAttack() const
{
	LOG_MSG(TEXT("AbleAttack"))
	if (CurGun == nullptr)
	{
		return false;
	}
	int32 CurAmmo = CurGun->GetLoadedAmmo();
	return CurAmmo > 0;
}

void ABLeonGun::Attack()
{
	Super::Attack();
	Shoot();
}

bool ABLeonGun::AbleReload() const
{
	LOG_MSG(TEXT("AbleReload"))

	if (InventoryInst == nullptr)
	{
		LOG_ERROR(TEXT("InventoryInst == nullptr"))
		return false;
	}

	if (CurGun == nullptr)
	{
		LOG_ERROR(TEXT("CurGun == nullptr"))
			return false;
	}

	int32 CurAmmo = CurGun->GetLoadedAmmo();
	int32 MaxAmmo = CurGun->GetMaxAmmo();
	int32 StoredAmmo = InventoryInst->GetItemCount(AmmoType);
	return CurAmmo < MaxAmmo && StoredAmmo > 0;
}

void ABLeonGun::Reload()
{
	if (InventoryInst == nullptr)
	{
		LOG_ERROR(TEXT("InventoryInst == nullptr"))
		return;
	}

	if (CurGun == nullptr)
	{
		LOG_ERROR(TEXT("CurGun == nullptr"))
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
	CurAmmo += ReloadAmmoCount;
	CurGun->SetLoadedAmmo(CurAmmo);
	InventoryInst->RemoveItem(AmmoType, ReloadAmmoCount);
	LOG_MSG(TEXT("Reload"))
}

uint32 ABLeonGun::GetAmmo() const
{
	if (InventoryInst == nullptr)
	{
		return UINT32_MAX;
	}

	return CurGun->GetLoadedAmmo();
}

void ABLeonGun::Shoot()
{
	LOG_MSG(TEXT("Shoot"))

	if (InventoryInst == nullptr)
	{
		LOG_ERROR(TEXT("InventoryInst == nullptr"))
			return;
	}

	if (CurGun == nullptr)
	{
		LOG_ERROR(TEXT("CurGun == nullptr"))
			return;
	}

	CamLineTraceStart = PlayerCamManager->GetCameraLocation();
	CamLineTraceEnd = CamLineTraceStart + PlayerCamManager->GetActorForwardVector() * Range;

	FCollisionQueryParams LineTraceParams;
	LineTraceParams.AddIgnoredActor(this);
	LineTraceParams.AddIgnoredActor(Player);

	bool bIsTarget = GetWorld()->LineTraceSingleByChannel(CamHitInfo, CamLineTraceStart, CamLineTraceEnd, ECollisionChannel::ECC_GameTraceChannel6, LineTraceParams);
	GunLineTraceStart = GetActorLocation();
	if (bIsTarget)
	{
		LOG_MSG(TEXT("There Is Target"))
		DrawDebugLine(GetWorld(), CamLineTraceStart, CamLineTraceEnd, FColor::Blue, true, 0.1, (uint8)0U, 1.f);
		
		GunLineTraceEnd = (CamHitInfo.ImpactPoint - GunLineTraceStart);
		GunLineTraceEnd.Normalize();
		GunLineTraceEnd *= Range;
		GunLineTraceEnd += GunLineTraceStart;
	}
	else
	{
		LOG_MSG(TEXT("There Is No Target"))
		GunLineTraceEnd = CamLineTraceEnd;
	}	

	bool bIsHit = GetWorld()->LineTraceSingleByChannel(GunHitInfo, GunLineTraceStart, GunLineTraceEnd, ECollisionChannel::ECC_GameTraceChannel4, LineTraceParams);
	if (bIsHit)
	{
		LOG_MSG(TEXT("AttackSuccess"))
		DrawDebugLine(GetWorld(), GunLineTraceStart, GunLineTraceEnd, FColor::Green, true);
		
		AActor* DamagedActor = GunHitInfo.GetActor();
		GunDamageEvent.HitInfo = GunHitInfo;
		GunDamageEvent.ShotDirection = Player->GetActorForwardVector();

		float CalDamage = DefaultDamage * DamageUnit;
		LOG_MSG(TEXT("Damage : %f"), CalDamage);

		UGameplayStatics::ApplyPointDamage(DamagedActor, CalDamage, Player->GetActorForwardVector(), GunDamageEvent.HitInfo, Player->GetController(), this, UBDMGPlayerDamage::StaticClass());
	}
	else
	{
		LOG_MSG(TEXT("AttackFail"))
		DrawDebugLine(GetWorld(), GunLineTraceStart, GunLineTraceEnd, FColor::Red, true);
	}

	int32 CurAmmo = CurGun->GetLoadedAmmo();
	CurGun->SetLoadedAmmo(--CurAmmo);
	LOG_MSG(TEXT("CurAmmo : %d"), CurAmmo)

	FireStart();
}

void ABLeonGun::DropShell()
{
	LOG_MSG(TEXT("DropShell"))
}

void ABLeonGun::DropMagazine()
{
	LOG_MSG(TEXT("DropMagazine"))
}

void ABLeonGun::FireStart()
{
	if (InventoryInst == nullptr)
	{
		LOG_ERROR(TEXT("InventoryInst == nullptr"))
			return;
	}

	if (CurGun == nullptr)
	{
		LOG_ERROR(TEXT("CurGun == nullptr"))
			return;
	}

	int32 CurAmmo = CurGun->GetLoadedAmmo();
	if (CurAmmo != 0)
	{
		CurState = EGunState::EAmmo_Fire_Ammo;
		LOG_MSG(TEXT("CurState : EAmmo_Fire_Ammo"))
	}
	else
	{
		CurState = EGunState::EAmmo_Fire_NoAmmo;
		LOG_MSG(TEXT("CurState : EAmmo_Fire_NoAmmo"))
	}
}

void ABLeonGun::FireEnd()
{
	if (InventoryInst == nullptr)
	{
		LOG_ERROR(TEXT("InventoryInst == nullptr"))
			return;
	}

	if (CurGun == nullptr)
	{
		LOG_ERROR(TEXT("CurGun == nullptr"))
			return;
	}

	int32 CurAmmo = CurGun->GetLoadedAmmo();
	if (CurAmmo == 0)
	{
		CurState = EGunState::ENoAmmo_Loop;
		LOG_MSG(TEXT("CurState : ENoAmmo_Loop"))
	}
	else
	{
		CurState = EGunState::EIdle_Loop;
		LOG_MSG(TEXT("CurState : EIdle_Loop"))
	}
}

void ABLeonGun::ReloadStart()
{
	if (InventoryInst == nullptr)
	{
		LOG_ERROR(TEXT("InventoryInst == nullptr"))
			return;
	}

	if (CurGun == nullptr)
	{
		LOG_ERROR(TEXT("CurGun == nullptr"))
			return;
	}

	int32 CurAmmo = CurGun->GetLoadedAmmo();
	if (CurAmmo == 0)
	{
		CurState = EGunState::ENoAmmo_Reload;
		LOG_MSG(TEXT("CurState : ENoAmmo_Reload"))
	}
	else
	{
		CurState = EGunState::EAmmo_Reload;
		LOG_MSG(TEXT("CurState : EAmmo_Reload"))
	}
}

void ABLeonGun::ReloadEnd()
{
	CurState = EGunState::EPutout;
	LOG_MSG(TEXT("CurState : EPutout"))
}

void ABLeonGun::PutoutEnd()
{
	CurState = EGunState::EIdle_Loop;
	LOG_MSG(TEXT("CurState : EIdle_Loop"))
}
