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
	EReload_Loop UMETA(DisplayName = "Reload_Loop"),
	EReload_End UMETA(DisplayName = "Reload_End")
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
	virtual void FireStart();
	virtual void FireEnd();
	virtual void ReloadStart() override;
	virtual void ReloadEnd();
	virtual void ReloadEndAfter() {}
	virtual void PutoutEnd();

	//Drop
	virtual void DropShell();
	virtual void DropMagazine();

	UFUNCTION(BlueprintNativeEvent)
	void SpawnShell();
	virtual void SpawnShell_Implementation();

	UFUNCTION(BlueprintNativeEvent)
	void SpawnMagazine();
	virtual void SpawnMagazine_Implementation();

	void PlayShootingSound();
	void PlayReload1Sound();
	void PlayReload2Sound();
	void PlayMagazine1Sound();
	void PlayMagazine2Sound();

	//Effect

	UFUNCTION(BlueprintNativeEvent)
	void PlayShootEffect();
	virtual void PlayShootEffect_Implementation();

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
	
	// _______________________Sound
	UPROPERTY(EditDefaultsOnly, Category = Sound)
	USoundBase* Sound_Shoot = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = Sound)
	USoundBase* Sound_Reload1 = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = Sound)
	USoundBase* Sound_Reload2 = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = Sound)
	USoundBase* Sound_Reload_Magazine1 = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = Sound)
	USoundBase* Sound_Reload_Magazine2 = nullptr;
};
