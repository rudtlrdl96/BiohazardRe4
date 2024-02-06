// Fill out your copyright notice in the Description page of Project Settings.

#include "Actor/Player/Weapon/Gun/BLeonGun.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/DamageEvents.h"
#include "DamageType/BDMGPlayerDamage.h"



void ABLeonGun::BeginPlay()
{
	Super::BeginPlay();
	PlayerCamManager = UGameplayStatics::GetPlayerCameraManager(this, 0);
}

bool ABLeonGun::AbleAttack() const
{
	LOG_MSG(TEXT("AbleAttack"))
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
	return CurAmmo < MaxAmmo && ExtraAmmo > 0;
}

void ABLeonGun::Reload()
{
	int32 ReloadAmmoCount = MaxAmmo - CurAmmo;
	if (ReloadAmmoCount > ExtraAmmo)
	{
		ReloadAmmoCount = ExtraAmmo;
	}
	CurAmmo += ReloadAmmoCount;
	ExtraAmmo -= ReloadAmmoCount;
	LOG_MSG(TEXT("Reload"))

}

void ABLeonGun::Shoot()
{
	LOG_MSG(TEXT("Shoot"))
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
		//DamagedActor->TakeDamage(CalDamage, GunDamageEvent, Player->GetController(), this);
	}
	else
	{
		LOG_MSG(TEXT("AttackFail"))
		DrawDebugLine(GetWorld(), GunLineTraceStart, GunLineTraceEnd, FColor::Red, true);
	}

	int32 PrevAmmo = CurAmmo--;
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
