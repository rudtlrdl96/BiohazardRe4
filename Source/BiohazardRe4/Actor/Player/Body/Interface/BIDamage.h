// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "BIDamage.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UBIDamage : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class BIOHAZARDRE4_API IBIDamage
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void DamageEnd() = 0;
	virtual void DeathEnd() = 0;
};
