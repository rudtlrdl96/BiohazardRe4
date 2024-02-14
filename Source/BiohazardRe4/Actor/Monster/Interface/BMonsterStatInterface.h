// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "BMonsterStatInterface.generated.h"

UINTERFACE(MinimalAPI)
class UBMonsterStatInterface : public UInterface
{
	GENERATED_BODY()
};

class BIOHAZARDRE4_API IBMonsterStatInterface
{
	GENERATED_BODY()

public:
	virtual float GetRunSpeed() const = 0;
	virtual float GetWalkSpeed() const = 0;
	virtual float GetAttackRadius() const = 0;
	virtual float GetDetectRadius() const = 0;
	virtual float GetPatrolRadius() const = 0;
	virtual float GetGroggyThreshold() const = 0;
	virtual float GetAttackSweepRadius() const = 0;
	virtual float GetWalkDistanceThreshold() const = 0;
};


