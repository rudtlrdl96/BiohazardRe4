// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/DamageType.h"
#include "..\Actor\Player\Body\BLeon.h"
#include "..\Item\ItemData.h"
#include "BDMGPlayerDamage.generated.h"

UENUM(BlueprintType)
enum class EPlayerDamageType : uint8
{
	Gun			UMETA(DisplayName = "Small"),
	Knife		UMETA(DisplayName = "Medium"),
	Kick		UMETA(DisplayName = "Large"),
	Grenade		UMETA(DisplayName = "ExLarge"),
};

/**
 * 
 */
UCLASS()
class BIOHAZARDRE4_API UBDMGPlayerDamage : public UDamageType
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, Category = Damage)
	EItemCode WeaponItemCode;

	UPROPERTY(EditAnywhere, Category = Damage)
	EPlayerDamageType DamageType;

};
