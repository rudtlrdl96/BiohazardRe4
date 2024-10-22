// Fill out your copyright notice in the Description page of Project Settings.


#include "BAIChainsawManController.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"

#include "BiohazardRe4.h"
#include "GameFramework/Character.h"
#include "Actor/Monster/Define/MonsterDefine.h"
#include "Actor/Monster/Interface/BMonsterAnimInterface.h"

ABAIChainsawManController::ABAIChainsawManController()
{
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<UBlackboardData> BBAssetRef(TEXT("/Script/AIModule.BlackboardData'/Game/Blueprints/Actor/Monster/BehaviorTree/BB_ChainsawMan.BB_ChainsawMan'"));

	if (BBAssetRef.Object != nullptr)
	{
		BBAsset = BBAssetRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BTAssetRef(TEXT("/Script/AIModule.BehaviorTree'/Game/Blueprints/Actor/Monster/BehaviorTree/BT_ChainsawMan.BT_ChainsawMan'"));
	
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
		Blackboard->SetValueAsBool(BBKEY_ISDAMAGED, false);

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

void ABAIChainsawManController::OnUnPossess()
{
	Super::OnUnPossess();

	StopAI();
}
