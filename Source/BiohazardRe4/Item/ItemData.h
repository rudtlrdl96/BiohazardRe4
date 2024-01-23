// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemData.generated.h"

UENUM(BlueprintType)
enum class EItemCode : uint8
{
	// ����
	Handgun_SR09R	UMETA(DisplayName = "Handgun"),	// ���� (���۹���)
	Shotgun_W870	UMETA(DisplayName = "Shotgun"),	// ����
	Rifle_SRM1903	UMETA(DisplayName = "Rifle"),	// ����
	CombatKnife		UMETA(DisplayName = "Combat Knife"),	// ������
	// ź��
	HandgunAmmo		UMETA(DisplayName = "Handgun Ammo"),	// ����ź
	ShotgunShells	UMETA(DisplayName = "Shotgun Ammo"),	// ����ź
	RifleAmmo		UMETA(DisplayName = "Rifle Ammo"),		// ����ź
	// ȸ��
	GreenHerb		UMETA(DisplayName = "Green Herb"),		// ��� ����
	RedHerb			UMETA(DisplayName = "Red Herb"),		// ���� ����	 
	MixedHerb_GG	UMETA(DisplayName = "MixedHerb (G + G)"),	// ȥ�� ���� (�� + ��)
	MixedHerb_GR	UMETA(DisplayName = "MixedHerb (G + R)"),	// ȥ�� ���� (�� + ��)
	FirstAidSpray	UMETA(DisplayName = "First Aid Spary (Max Heal)"),	// ���� ��������
	// ���վ�����	
	Gunpowder		UMETA(DisplayName = "Gunpowder"),		// ȭ��
	Resources_S		UMETA(DisplayName = "Resources S"),	// ���� S
	Resources_L		UMETA(DisplayName = "Resources L"),	// ���� L
};

/*
* ������ ���� ����ü
*/

USTRUCT(BlueprintType)
struct FBItemData : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

	FBItemData()
		: ItemName(""), ItemSize(1, 1)
	{

	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EItemCode ItemCode;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName ItemName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString ItemInformation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FIntPoint ItemSize;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Data")
	int32 MaxCount;	// �ִ� ����� ������ �� �ִ���

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Resource")
	UStaticMesh* Mesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Resource")
	FVector Location;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Resource")
	FRotator Rotation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Resource")
	FVector Scale;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Resource")
	FVector TurnLocation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Resource")
	FRotator TurnRotation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Resource")
	FVector UIPivot;
};
