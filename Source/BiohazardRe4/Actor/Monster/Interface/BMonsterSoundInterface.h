// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "BMonsterSoundInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UBMonsterSoundInterface : public UInterface
{
	GENERATED_BODY()
};

UENUM()
enum class ESoundType
{
	Idle,
	Yell,
	Detect,
	Damaged,
	GeneralGroggy,
	HeadShotGroggy,
};

class BIOHAZARDRE4_API IBMonsterSoundInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	virtual void PlaySound(ESoundType _PlaySoundType) = 0;
	virtual bool isAblePlay(ESoundType _PlaySoundType) = 0;
};
