// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InventoryItem.h"
#include "Containers/Union.h"
#include "InventoryWeapon.generated.h"

UCLASS()
class BIOHAZARDRE4_API ABInventoryWeapon : public ABInventoryItem
{
	GENERATED_BODY()
	
	typedef union WeaponData
	{
		int32 Durability;
		int32 Ammo;
	} WeaponData;
	WeaponData MainData;
	WeaponData SubData;

public:

	// 가방에 남은 탄
	UFUNCTION(BlueprintCallable)
	int32 GetStoredAmmo() const;

	// 장전된 탄
	UFUNCTION(BlueprintCallable)
	int32 GetLoadedAmmo() const
	{
		return MainData.Ammo;
	}
	UFUNCTION(BlueprintCallable)
	void SetLoadedAmmo(int32 Ammo)
	{
		MainData.Ammo = Ammo;
	}
	// 최대 장탄수
	UFUNCTION(BlueprintCallable)
	int32 GetMaxAmmo() const
	{
		return SubData.Ammo;
	}
	UFUNCTION(BlueprintCallable)
	void SetMaxAmmo(int32 Ammo)
	{
		SubData.Ammo = Ammo;
	}

	// 칼 현재 내구도 퍼센트
	UFUNCTION(BlueprintCallable)
	float GetDurabilityPercent() const
	{
		return (float)MainData.Durability / SubData.Durability;
	}

	// 칼 현재 내구도
	UFUNCTION(BlueprintCallable)
	int32 GetDurability() const
	{
		return MainData.Durability;
	}
	// 칼 내구도 지정
	UFUNCTION(BlueprintCallable)
	void SetDurability(float Durability)
	{
		MainData.Durability = Durability;
	}
	// 칼 최대 내구도
	UFUNCTION(BlueprintCallable)
	int32 GetMaxDurability() const
	{
		return SubData.Durability;
	}
	// 칼 최대 내구도 지정
	UFUNCTION(BlueprintCallable)
	void SetMaxDurability(float Durability)
	{
		SubData.Durability = Durability;
	}
};
