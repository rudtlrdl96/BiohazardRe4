// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemData.generated.h"

UENUM(BlueprintType)
enum class EItemCode : uint8
{
	// 무기
	Handgun_SR09R	UMETA(DisplayName = "Handgun"),	// 권총 (시작무기)
	Shotgun_W870	UMETA(DisplayName = "Shotgun"),	// 샷건
	Rifle_SRM1903	UMETA(DisplayName = "Rifle"),	// 소총
	CombatKnife		UMETA(DisplayName = "Combat Knife"),	// 나이프
	// 탄약
	HandgunAmmo		UMETA(DisplayName = "Handgun Ammo"),	// 권총탄
	ShotgunShells	UMETA(DisplayName = "Shotgun Ammo"),	// 샷건탄
	RifleAmmo		UMETA(DisplayName = "Rifle Ammo"),		// 소총탄
	// 회복
	GreenHerb		UMETA(DisplayName = "Green Herb"),		// 녹색 약초
	RedHerb			UMETA(DisplayName = "Red Herb"),		// 빨간 약초	 
	MixedHerb_GG	UMETA(DisplayName = "MixedHerb (G + G)"),	// 혼합 약초 (녹 + 녹)
	MixedHerb_GR	UMETA(DisplayName = "MixedHerb (G + R)"),	// 혼합 약초 (녹 + 적)
	FirstAidSpray	UMETA(DisplayName = "First Aid Spary (Max Heal)"),	// 구급 스프레이
	// 조합아이템	
	Gunpowder		UMETA(DisplayName = "Gunpowder"),		// 화약
	Resources_S		UMETA(DisplayName = "Resources S"),	// 소재 S
	Resources_L		UMETA(DisplayName = "Resources L"),	// 소재 L
};

/*
* 아이템 정보 구조체
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
	int32 MaxCount;	// 최대 몇개까지 보유할 수 있는지

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
