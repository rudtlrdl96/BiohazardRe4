// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/DamageType.h"
#include "BDMGMonsterDamage.generated.h"

/**
 * 
 */

UENUM(BlueprintType)
enum class EMonsterDamageDirection : uint8
{
	Top		UMETA(DisplayName = "Top"),
	Bottom	UMETA(DisplayName = "Bottom"),
};

UENUM(BlueprintType)
enum class EMonsterDamagePower : uint8
{
	Small		UMETA(DisplayName = "Small"),
	Medium		UMETA(DisplayName = "Medium"),
	Large		UMETA(DisplayName = "Large"),
	ExLarge		UMETA(DisplayName = "ExLarge"),
};

UCLASS()
class BIOHAZARDRE4_API UBDMGMonsterDamage : public UDamageType
{
	GENERATED_BODY()
public:
	UBDMGMonsterDamage() {}

	UPROPERTY(EditAnywhere, Category = Damage)
	EMonsterDamageDirection DamageDirection = EMonsterDamageDirection::Top;

	UPROPERTY(EditAnywhere, Category = Damage)
	EMonsterDamagePower DamagePower = EMonsterDamagePower::Small;
};
