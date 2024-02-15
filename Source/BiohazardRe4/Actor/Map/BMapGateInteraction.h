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

	virtual void MapObjOpen() override;
	virtual void MapObjClose() override;

};
