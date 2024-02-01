// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_Wait.h"
#include "BBTTask_MonsterWait.generated.h"

/**
 * 
 */
UCLASS()
class BIOHAZARDRE4_API UBBTTask_MonsterWait : public UBTTask_Wait
{
	GENERATED_BODY()
public:
	UBBTTask_MonsterWait();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult) override;
};
