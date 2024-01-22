// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "BMonsterStatInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UBMonsterStatInterface : public UInterface
{
	GENERATED_BODY()
};
/**
 * 
 */
class BIOHAZARDRE4_API IBMonsterStatInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual float GetAttackRange() = 0;
	virtual float GetWeaponAttackRadius() = 0;
};


