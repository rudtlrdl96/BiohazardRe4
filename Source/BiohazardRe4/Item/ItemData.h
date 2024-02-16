// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemData.generated.h"

UENUM(BlueprintType)
enum class EItemCode : uint8
{
	// null üũ�� Empty �����Ͱ� ��������� ǥ��
	Empty			UMETA(DisplayName = "Empty"),
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
	// ����ź
	Grenade			UMETA(DisplayName = "Grenade"),     // ����ź
	Flashbang		UMETA(DisplayName = "Flashbang"),   // ����ź

	Money			UMETA(DisplayName = "Money"),   // ��
	End,
};

/*
* ������ ���� ����ü
*/

USTRUCT(BlueprintType)
struct FBItemData : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EItemCode ItemCode = EItemCode::Empty;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName ItemName = "";

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString ItemInformation = "";

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FIntPoint ItemSize = FIntPoint(0, 0);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Data")
	int32 MaxCount = 1;	// �ִ� ����� ������ �� �ִ���

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Resource")
	UStaticMesh* Mesh = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Resource")
	FVector Location = FVector::ZeroVector;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Resource")
	FRotator Rotation = FRotator::ZeroRotator;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Resource")
	FVector Scale = FVector::OneVector;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Resource")
	FVector TurnLocation = FVector::ZeroVector;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Resource")
	FRotator TurnRotation = FRotator::ZeroRotator;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Data")
	int32 MagazineCapacity = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Resource")
	class UPaperSprite* Icon = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Store")
	FVector StoreLocation = FVector::ZeroVector;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Store")
	FRotator StoreRotation = FRotator::ZeroRotator;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Store")
	FVector StoreScale = FVector::OneVector;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Store")
	int Price = 0;
};
