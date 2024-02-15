// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actor/Map/BMapBaseInteraction.h"
#include "BMapDrawerInteraction.generated.h"

/**
 * 
 */
UCLASS()
class BIOHAZARDRE4_API ABMapDrawerInteraction : public ABMapBaseInteraction
{
	GENERATED_BODY()
public:
	ABMapDrawerInteraction();

	UPROPERTY(EditAnywhere, Category = "Open")
	UBoxComponent* ATrigger = nullptr;
	virtual EInteraction GetInteractionType() const
	{
		return EInteraction::OpenDrawer;
	}
	virtual bool AbleInteraction() const override
	{
		return bInter;
	}


protected:
};

