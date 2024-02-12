// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actor/Map/BJumpObstacleLineTrigger.h"
#include "BCliffLineTrigger.generated.h"

/**
 * 
 */
UCLASS()
class BIOHAZARDRE4_API ABCliffLineTrigger : public ABJumpObstacleLineTrigger
{
	GENERATED_BODY()

public:
	virtual FJumpData GetJumpMetaData(const FVector& _Location) const override;

	virtual EInteraction GetInteractionType() const override
	{
		return EInteraction::FallCliff;
	}
};