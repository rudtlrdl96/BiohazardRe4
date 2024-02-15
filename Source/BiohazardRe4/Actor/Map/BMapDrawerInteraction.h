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
	virtual void MapObjOpen() override;
	virtual void MapObjClose() override;
};
