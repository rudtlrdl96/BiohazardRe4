// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "BLeonStat.generated.h"

/**
 * 
 */
UCLASS()
class BIOHAZARDRE4_API UBLeonStat : public UDataAsset
{
	GENERATED_BODY()
public:
	UPROPERTY(EditDefaultsOnly, Category = Stat)
	float CurrentHp = 1000.0f;
	UPROPERTY(EditDefaultsOnly, Category = Stat)
	float MaxHp = 1000.0f;	
	UPROPERTY(EditDefaultsOnly, Category = Stat)
	float Damage = 100.0f;

};
