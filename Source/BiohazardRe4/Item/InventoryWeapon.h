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

	// ���濡 ���� ź
	UFUNCTION(BlueprintCallable)
	int32 GetStoredAmmo() const;

	// ������ ź
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
	// �ִ� ��ź��
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

	// Į ���� ������ �ۼ�Ʈ
	UFUNCTION(BlueprintCallable)
	float GetDurabilityPercent() const
	{
		return (float)MainData.Durability / SubData.Durability;
	}

	// Į ���� ������
	UFUNCTION(BlueprintCallable)
	int32 GetDurability() const
	{
		return MainData.Durability;
	}
	// Į ������ ����
	UFUNCTION(BlueprintCallable)
	void SetDurability(float Durability)
	{
		MainData.Durability = Durability;
	}
	// Į �ִ� ������
	UFUNCTION(BlueprintCallable)
	int32 GetMaxDurability() const
	{
		return SubData.Durability;
	}
	// Į �ִ� ������ ����
	UFUNCTION(BlueprintCallable)
	void SetMaxDurability(float Durability)
	{
		SubData.Durability = Durability;
	}
};
