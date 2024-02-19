// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actor/Map/BMapBaseInteraction.h"
#include "BMapDestructionInteraction.generated.h"

/**
 * 
 */
UCLASS()
class BIOHAZARDRE4_API ABMapDestructionInteraction : public ABMapBaseInteraction
{
	GENERATED_BODY()
	virtual EInteraction GetInteractionType() const
	{
		return EInteraction::BreakBox;
	}
};
