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
	Mutter,
	Yell,
	Detect,
	Damaged,
	Attack,
	Parry,
	Kicked,
	GeneralGroggy,
	HeadShotGroggy,
	PointDeath,
};

UENUM()
enum class EFootSoundType
{
	Dirt,
	Water,
	Grass,
	Wood,
	Rock,
};

class BIOHAZARDRE4_API IBMonsterSoundInterface
{
	GENERATED_BODY()
public:

	virtual void PlaySound(ESoundType _PlaySoundType) = 0;
	virtual bool isAblePlay(ESoundType _PlaySoundType) = 0;

	virtual void PlayFootSound(EFootSoundType _PlaySoundType, bool _isLeft) = 0;
};
