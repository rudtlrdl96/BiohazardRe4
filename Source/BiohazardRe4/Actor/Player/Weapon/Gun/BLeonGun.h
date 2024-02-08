// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actor/Player/Weapon/BLeonWeapon.h"
#include "BLeonGun.generated.h"

UENUM(BluePrintType)
enum class EGunState : uint8
{
	EIdle_Loop UMETA(DisplayName = "Idle_Loop"),
	ENoAmmo_Loop UMETA(DisplayName = "NoAmmo_Loop"),
	EAmmo_Fire_Ammo UMETA(DisplayName = "Ammo_Fire_Ammo"),
	EAmmo_Fire_NoAmmo UMETA(DisplayName = "Ammo_Fire_NoAmmo"),
	ENoAmmo_Fire_NoAmmo UMETA(DisplayName = "NoAmmo_Fire_NoAmmo"),
	EAmmo_Reload UMETA(DisplayName = "Ammo_Reload"),
	ENoAmmo_Reload UMETA(DisplayName = "NoAmmo_Reload"),
	EPutout UMETA(DisplayName = "Putout"),
};

UCLASS()
class BIOHAZARDRE4_API ABLeonGun : public ABLeonWeapon
{
	GENERATED_BODY()

public:
	ABLeonGun();

	virtual bool AbleAttack() const override;
	virtual void Attack() override;

	virtual bool AbleReload() const override;
	virtual void Reload() override;
	virtual uint32 GetAmmo() const override;

	float GetFireRate() const
	{
		return RateOfFire;
	}

	UFUNCTION(BlueprintCallable)
	EGunState GetCurState() const
	{
		return CurState;
	}

	// State
	void FireStart();
	void FireEnd();
	void ReloadStart() override;
	void ReloadEnd();
	void PutoutEnd();

	//Drop
	virtual void DropShell();
	virtual void DropMagazine();

protected:
	virtual void BeginPlay() override;

	virtual void Shoot();


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
	FPointDamageEvent GunDamageEvent;

	UPROPERTY(BlueprintReadWrite)
	EGunState CurState = EGunState::EIdle_Loop;

	EItemCode AmmoType = EItemCode::End;

	class ABInventoryWeapon* CurGun = nullptr;

	void SetCurLoopState();

};
