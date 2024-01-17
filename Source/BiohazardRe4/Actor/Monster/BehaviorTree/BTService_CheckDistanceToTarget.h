// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_CheckDistanceToTarget.generated.h"

/**
 * 
 */
UCLASS()
class BIOHAZARDRE4_API UBTService_CheckDistanceToTarget : public UBTService
{
	GENERATED_BODY()

public:
	UBTService_CheckDistanceToTarget();
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
