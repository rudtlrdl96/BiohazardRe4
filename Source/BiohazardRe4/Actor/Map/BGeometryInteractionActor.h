// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GeometryCollection/GeometryCollectionActor.h"
#include "Actor/Generic/Interface/BInteraction.h"
#include "BGeometryInteractionActor.generated.h"

/**
 * 
 */
UCLASS()
class BIOHAZARDRE4_API ABGeometryInteractionActor : public AGeometryCollectionActor, public IBInteraction
{
	GENERATED_BODY()
	
public:
	// 현재 상호작용이 가능한 상태인지
	virtual bool AbleInteraction() const override;

	// 현재 상호작용의 상태를 반환
	virtual EInteraction GetInteractionType() const override;
};
