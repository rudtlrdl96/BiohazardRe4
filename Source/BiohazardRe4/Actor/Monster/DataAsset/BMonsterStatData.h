// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "BMonsterStatData.generated.h"

/**
 * 
 */
UCLASS()
class BIOHAZARDRE4_API UBMonsterStatData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Category = Stat)
	float MaxHp = 0.0f;

	UPROPERTY(EditDefaultsOnly, Category = Stat)
	float CurrentHp = 0.0f;

	UPROPERTY(EditDefaultsOnly, Category = Stat)
	float DetectRadius = 0.0f;

	UPROPERTY(EditDefaultsOnly, Category = Stat)
	float PatrolRadius = 0.0f;

	UPROPERTY(EditDefaultsOnly, Category = Stat)
	float RunSpeed = 0.0f;

	UPROPERTY(EditDefaultsOnly, Category = Stat)
	float WalkSpeed = 0.0f;

	UPROPERTY(EditDefaultsOnly, Category = Stat)
	float WalkDistanceThreshold = 0.0f;

	UPROPERTY(EditDefaultsOnly, Category = Stat)
	float BaseAttackPower = 0.0f;

	UPROPERTY(EditDefaultsOnly, Category = Stat)
	float GroggyThreshold = 0.0f;

	UPROPERTY(EditDefaultsOnly, Category = Stat)
	float AttackRange = 0.0f;
};
