// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Player/Weapon/Gun/BLeonGun.h"
#include "Kismet/GameplayStatics.h"
#include "BiohazardRe4.h"

void ABLeonGun::BeginPlay()
{
	Super::BeginPlay();
	PlayerCamManager = UGameplayStatics::GetPlayerCameraManager(this, 0);
}

bool ABLeonGun::AbleAttack() const
{
	LOG_MSG(TEXT("AbleAttack"))
	return true;
}

void ABLeonGun::Attack()
{
	Super::Attack();
	Shoot();
}

bool ABLeonGun::AbleReload() const
{
	LOG_MSG(TEXT("AbleReload"))
		return true;
}

void ABLeonGun::Reload()
{
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
		AActor* ShootedActor = GunHitInfo.GetActor();
	}
	else
	{
		LOG_MSG(TEXT("AttackFail"))
		DrawDebugLine(GetWorld(), GunLineTraceStart, GunLineTraceEnd, FColor::Red, true);
	}
}

void ABLeonGun::DropShell()
{
	LOG_MSG(TEXT("DropShell"))

}

void ABLeonGun::DropMagazine()
{
	LOG_MSG(TEXT("DropMagazine"))

}