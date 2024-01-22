// Fill out your copyright notice in the Description page of Project Settings.


#include "BAIBasicMonsterController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "../Define/MonsterDefine.h"

ABAIBasicMonsterController::ABAIBasicMonsterController()
{
	static ConstructorHelpers::FObjectFinder<UBlackboardData> BBAssetRef(TEXT("/Script/AIModule.BlackboardData'/Game/Blueprints/Actor/Monster/BehaviorTree/BB_BasicMonser.BB_BasicMonser'"));

	if (BBAssetRef.Object != nullptr)
	{
		BBAsset = BBAssetRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BTAssetRef(TEXT("/Script/AIModule.BehaviorTree'/Game/Blueprints/Actor/Monster/BehaviorTree/BT_BasicMonster.BT_BasicMonster'"));

	if (BTAssetRef.Object != nullptr)
	{
		BTAsset = BTAssetRef.Object;
	}
}

void ABAIBasicMonsterController::RunAI()
{
	UBlackboardComponent* BlackboardPtr = Blackboard.Get();

	if (UseBlackboard(BBAsset, BlackboardPtr) != false)
	{
		Blackboard->SetValueAsVector(BBKEY_PATROLPOS, GetPawn()->GetActorLocation());
		bool RunResult = RunBehaviorTree(BTAsset);
		ensure(RunResult);
	}
}

void ABAIBasicMonsterController::StopAI()
{
	UBehaviorTreeComponent* BTComponent = Cast<UBehaviorTreeComponent>(BrainComponent);

	if (BTComponent != nullptr)
	{
		BTComponent->StopTree();
	}
}

void ABAIBasicMonsterController::OnPossess(APawn* _InPawn)
{
	Super::OnPossess(_InPawn);

	RunAI();
}
