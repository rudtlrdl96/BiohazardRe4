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

	UFUNCTION()
	void OnTargetPerceptionUpdated(AActor* _Actor, FAIStimulus const _Stimulus);
	
	UFUNCTION()
	void OnTargetPerceptionForgotten(AActor* _Actor);

protected:
	virtual void OnPossess(APawn* _InPawn) override;
	virtual void OnUnPossess() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
	class UAIPerceptionComponent* AIPerceptionComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
	class UAISenseConfig_Sight* SightConfig;

private:
	UPROPERTY()
	TObjectPtr<class UBlackboardData> BBAsset;

	UPROPERTY()
	TObjectPtr<class UBehaviorTree> BTAsset;

	void InitPerceptionSystem(class UBMonsterStatData* _InData);
	void SetPerceptionSystem();

	void PlayDetectSound();
	void SetValue();
};
