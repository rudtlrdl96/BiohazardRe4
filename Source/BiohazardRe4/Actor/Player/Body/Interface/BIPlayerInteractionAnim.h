// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "BIPlayerInteractionAnim.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UBIPlayerInteractionAnim : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class BIOHAZARDRE4_API IBIPlayerInteractionAnim
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	virtual void GetItemEnd() = 0;
	virtual void JumpObstacleEnd() = 0;
	virtual void FallGravityActive() = 0;
	virtual void FallTraceActive() = 0;
	virtual void FallLandingEnd() = 0;
	virtual void GravityActive() = 0;
};
