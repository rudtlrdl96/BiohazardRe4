// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_Wait.h"
#include "BTTask_ChainsawManWait.generated.h"

/**
 * 
 */
UCLASS()
class BIOHAZARDRE4_API UBTTask_ChainsawManWait : public UBTTask_Wait
{
	GENERATED_BODY()
public:
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
