// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Monster/AIController/BAIChainsawManController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "../Define/MonsterDefine.h"

ABAIChainsawManController::ABAIChainsawManController()
{
	static ConstructorHelpers::FObjectFinder<UBlackboardData> BBAssetRef(TEXT("/Script/AIModule.BlackboardData'/Game/Blueprints/Actor/Monster/AIController/BB_ChainsawMan.BB_ChainsawMan'"));

	if (BBAssetRef.Object != nullptr)
	{
		BBAsset = BBAssetRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BTAssetRef(TEXT("/Script/AIModule.BehaviorTree'/Game/Blueprints/Actor/Monster/AIController/BT_ChainsawMan.BT_ChainsawMan'"));
	
	if (BTAssetRef.Object != nullptr)
	{
		BTAsset = BTAssetRef.Object;
	}
}

void ABAIChainsawManController::RunAI()
{
	UBlackboardComponent* BlackboardPtr = Blackboard.Get();
	
	if (UseBlackboard(BBAsset, BlackboardPtr) != false)
	{
		//두 번째 인자를 시작 위치로.
		Blackboard->SetValueAsVector(BBKEY_HOMEPOS, GetPawn()->GetActorLocation());

		bool RunResult = RunBehaviorTree(BTAsset);
		ensure(RunResult);
	}
}

void ABAIChainsawManController::StopAI()
{
	UBehaviorTreeComponent* BTComponent = Cast<UBehaviorTreeComponent>(BrainComponent);
	
	if (BTComponent != nullptr)
	{
		BTComponent->StopTree();
	}
}

void ABAIChainsawManController::OnPossess(APawn* _InPawn)
{
	Super::OnPossess(_InPawn);

	RunAI();
}
