// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "BAIBasicMonsterController.generated.h"

/**
 * 
 */
UCLASS()
class BIOHAZARDRE4_API ABAIBasicMonsterController : public AAIController
{
	GENERATED_BODY()
public:
	ABAIBasicMonsterController();


	void RunAI();
	void StopAI();

protected:
	virtual void OnPossess(APawn* _InPawn) override;

private:
	UPROPERTY()
	TObjectPtr<class UBlackboardData> BBAsset;

	UPROPERTY()
	TObjectPtr<class UBehaviorTree> BTAsset;
};
