// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actor/Player/Weapon/BLeonWeapon.h"
#include "BLeonGun.generated.h"

/**
 * 
 */
UCLASS()
class BIOHAZARDRE4_API ABLeonGun : public ABLeonWeapon
{
	GENERATED_BODY()

public:
	virtual bool AbleAttack() const override;
	virtual void Attack() override;

	virtual bool AbleReload() const override;
	virtual void Reload() override;

	float GetFireRate() const
	{
		return RateOfFire;
	}

	virtual uint32 GetAmmo() const override
	{
		return static_cast<uint32>(CurAmmo);
	}

protected:
	virtual void BeginPlay() override;

	virtual void Shoot();
	virtual void DropShell();
	virtual void DropMagazine();

	//  ______________________LineTraceOption_Camera
	UPROPERTY()
	APlayerCameraManager* PlayerCamManager = nullptr;

	UPROPERTY()
	FHitResult CamHitInfo;

	UPROPERTY()
	FVector CamLineTraceStart = FVector::Zero();

	UPROPERTY()
	FVector CamLineTraceEnd = FVector::Zero();


	//  ______________________LineTraceOption_Gun
	UPROPERTY()
	FHitResult GunHitInfo;

	UPROPERTY()
	FVector GunLineTraceStart = FVector::Zero();

	UPROPERTY()
	FVector GunLineTraceEnd = FVector::Zero();

	//  ______________________WeaponInfo_Common

	UPROPERTY()
	float DamageUnit = 120.f;

	UPROPERTY()
	float DefaultDamage = 1.f;

	UPROPERTY()
	float RateOfFire = 1.f;

	//  ______________________WeaponInfo_Gun
	UPROPERTY()
	float Range = 5000.f;

	UPROPERTY()
	int32 MaxAmmo = 5;

	UPROPERTY()
	int32 CurAmmo = 0;

	UPROPERTY()
	int32 ExtraAmmo = 100;

	UPROPERTY()
	FPointDamageEvent GunDamageEvent;

};
