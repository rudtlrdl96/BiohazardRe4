// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BBTService_SpeedUpdate.generated.h"

/**
 * 
 */
UCLASS()
class BIOHAZARDRE4_API UBBTService_SpeedUpdate : public UBTService
{
	GENERATED_BODY()
public:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
