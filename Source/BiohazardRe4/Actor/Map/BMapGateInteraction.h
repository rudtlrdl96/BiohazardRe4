// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actor/Map/BMapBaseInteraction.h"
#include "BMapGateInteraction.generated.h"

/**
 * 
 */
UCLASS()
class BIOHAZARDRE4_API ABMapGateInteraction : public ABMapBaseInteraction
{
	GENERATED_BODY()
public:
	ABMapGateInteraction();

	UPROPERTY(EditAnywhere, Category = "Open")
	UBoxComponent* ATrigger = nullptr;
	virtual EInteraction GetInteractionType() const
	{
		return EInteraction::OpenGate;
	}
	virtual bool AbleInteraction() const override
	{
		return false == bIsUse;
	}

	inline bool UseGate()
	{
		if (false == bIsUse)
		{
			bIsUse = true;
			return true;
		}

		return false;
	}

	inline void UseGateEnd()
	{
		bIsUse = false;
	}
protected:
	uint8 bIsUse = false;
};
