// Fill out your copyright notice in the Description page of Project Settings.


#include "BAIBasicMonsterController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "../Define/MonsterDefine.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AIPerceptionComponent.h"
#include "GameFramework/Character.h"
#include "BiohazardRe4.h"
#include "../Interface/BMonsterStatInterface.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "../DataAsset/BMonsterStatData.h"

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

	AIPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerceptionComponent"));
	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));

	static ConstructorHelpers::FObjectFinder<UBMonsterStatData> BasicMonsterStatDataRef(TEXT("/Script/BiohazardRe4.BMonsterStatData'/Game/Blueprints/Actor/Monster/DataAsset/DA_BasicMonsterStat.DA_BasicMonsterStat'"));

	if (BasicMonsterStatDataRef.Object != nullptr)
	{
		InitPerceptionSystem(BasicMonsterStatDataRef.Object);
	}

	SetPerceptionSystem();
}

void ABAIBasicMonsterController::RunAI()
{
	UBlackboardComponent* BlackboardPtr = Blackboard.Get();

	if (UseBlackboard(BBAsset, BlackboardPtr) != false)
	{
		Blackboard->SetValueAsVector(BBKEY_HOMEPOS, GetPawn()->GetActorLocation());
		Blackboard->SetValueAsBool(BBKEY_ISDAMAGED, false);
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

void ABAIBasicMonsterController::OnTargetPerceptionUpdated(AActor* _Actor, FAIStimulus const _Stimulus)
{
	APawn* UpdatedPawn = Cast<APawn>(_Actor);
		
	if (UpdatedPawn != nullptr && UpdatedPawn->GetController()->IsPlayerController() == true)
	{
		if (_Stimulus.WasSuccessfullySensed() == true)
		{
			GetBlackboardComponent()->SetValueAsBool(BBKEY_ISDETECTED, _Stimulus.WasSuccessfullySensed());
			GetBlackboardComponent()->SetValueAsObject(BBKEY_TARGET, UpdatedPawn);

			ACharacter* OwnerCharacter = GetCharacter();
			if (OwnerCharacter == nullptr)
			{
				LOG_WARNING(TEXT("OwnerCharacter is nullptr"));
				return;
			}

			OwnerCharacter->GetCharacterMovement()->bOrientRotationToMovement = true;
		}
	}
}

void ABAIBasicMonsterController::OnTargetPerceptionForgotten(AActor* _Actor)
{
	APawn* TargetPawn = Cast<APawn>(_Actor);

	if (TargetPawn != nullptr && TargetPawn->GetController()->IsPlayerController() == true)
	{
		GetBlackboardComponent()->SetValueAsBool(BBKEY_ISDETECTED, false);
		GetBlackboardComponent()->SetValueAsObject(BBKEY_TARGET, nullptr);

		return;
	}
}

void ABAIBasicMonsterController::OnPossess(APawn* _InPawn)
{
	Super::OnPossess(_InPawn);
	RunAI();
}

void ABAIBasicMonsterController::InitPerceptionSystem(UBMonsterStatData* _InData)
{
	if (_InData == nullptr)
	{
		LOG_FATAL(TEXT("_InData is nullptr"));
		return;
	}

	LOG_MSG(TEXT("PerceptionSystem is initted"));

	SightConfig->SightRadius = _InData->DetectRadius;
	SightConfig->PeripheralVisionAngleDegrees = 30.0f;

	//멀어지면 따라가지 않도록. 일단은 계속 따라가게 함.
	SightConfig->LoseSightRadius = _InData->DetectRadius;
	SightConfig->SetMaxAge(-1.0f);

	SightConfig->DetectionByAffiliation.bDetectEnemies = true;
	SightConfig->DetectionByAffiliation.bDetectFriendlies = true;
	SightConfig->DetectionByAffiliation.bDetectNeutrals = true;
}

void ABAIBasicMonsterController::SetPerceptionSystem()
{
	if (AIPerceptionComponent != nullptr)
	{
		LOG_MSG(TEXT("AI PerCeption Linked"));

		//펄셉션 컴포넌트가 어떤 감각에 통제될 것인가.
		AIPerceptionComponent->ConfigureSense(*SightConfig);
		AIPerceptionComponent->SetDominantSense(*SightConfig->GetSenseImplementation());

		//해당 컴포넌트를 AI컨트롤러에 세팅.
		SetPerceptionComponent(*AIPerceptionComponent);

		//델리게이트에 상황에 맞는 함수를 바인딩
		AIPerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &ABAIBasicMonsterController::OnTargetPerceptionUpdated);
		AIPerceptionComponent->OnTargetPerceptionForgotten.AddDynamic(this, &ABAIBasicMonsterController::OnTargetPerceptionForgotten);
	}
}
