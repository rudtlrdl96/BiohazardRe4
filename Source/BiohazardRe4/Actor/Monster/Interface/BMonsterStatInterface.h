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
	virtual float GetAttackRadius() = 0;
	virtual float GetAttackSweepRadius() = 0;
	virtual float GetDetectRadius() = 0;
	virtual float GetPatrolRadius() = 0;
};


