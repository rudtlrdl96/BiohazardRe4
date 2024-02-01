// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "BInterface_WeaponReload.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UBInterface_WeaponReload : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class BIOHAZARDRE4_API IBInterface_WeaponReload
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void ReloadStart() = 0;
	virtual void Reload() = 0;
	virtual void ReloadEnd() = 0;
	virtual bool AbleReload() const = 0;
};
