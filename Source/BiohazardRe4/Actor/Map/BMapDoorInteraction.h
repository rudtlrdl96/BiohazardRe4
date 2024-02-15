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
	virtual void MapObjOpen() override;
	virtual void MapObjClose() override;
};
