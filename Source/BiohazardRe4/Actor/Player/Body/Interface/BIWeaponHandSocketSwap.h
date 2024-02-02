// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "BIWeaponHandSocketSwap.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UBIWeaponHandSocketSwap : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class BIOHAZARDRE4_API IBIWeaponHandSocketSwap
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void AttachLeftHandSocket() = 0;
	virtual void AttachRightHandSocket() = 0;
};
