// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actor/Map/BMapBaseInteraction.h"
#include "BMapDoorInteraction.generated.h"

/**
 * 
 */
UCLASS()
class BIOHAZARDRE4_API ABMapDoorInteraction : public ABMapBaseInteraction
{
	GENERATED_BODY()
public:
	ABMapDoorInteraction();
	virtual EInteraction GetInteractionType() const
	{
		return EInteraction::OpenDrawer;
	}
	UFUNCTION(BlueprintCallable)
	bool IsBack() const
	{
		return bBack;
	}
	UFUNCTION(BlueprintCallable)
	bool IsFront() const
	{
		return bFront;
	}
	UFUNCTION(BlueprintCallable)
	bool IsInOutValue() const
	{
		return bInValue;
	}

	UPROPERTY(BlueprintReadWrite, Category = "InValue")
	uint8 bInValue : 1;
	UPROPERTY(BlueprintReadWrite, Category = "FrontValue")
	uint8 bFront : 1;
	UPROPERTY(BlueprintReadWrite, Category = "BackValue")
	uint8 bBack : 1;

};
