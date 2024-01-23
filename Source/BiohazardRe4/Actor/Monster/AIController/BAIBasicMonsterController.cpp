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

	//�Ʒ� �ڵ� ������ �ۿ��� �ϴ� ����� �ֳ� (�ν��Ͻ����� �ٸ��� �ϴ� ���)
	{
		SightConfig->SightRadius = 600.0f;
		SightConfig->PeripheralVisionAngleDegrees = 30.0f;
		
		//�־����� ������ �ʵ���. �ϴ��� ��� ���󰡰� ��.
		SightConfig->LoseSightRadius = 600.0f;
		SightConfig->SetMaxAge(0.0f);
		
		SightConfig->DetectionByAffiliation.bDetectEnemies = true;
		SightConfig->DetectionByAffiliation.bDetectFriendlies = true;
		SightConfig->DetectionByAffiliation.bDetectNeutrals = true;
		
		SetPerceptionSystem();
	}

	//SetPerceptionSystem();
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

void ABAIBasicMonsterController::OnTargetPerceptionUpdated(AActor* _Actor, FAIStimulus const _Stimulus)
{
	APawn* UpdatedPawn = Cast<APawn>(_Actor);
		
	if (UpdatedPawn != nullptr && UpdatedPawn->GetController()->IsPlayerController() == true)
	{
		if (_Stimulus.WasSuccessfullySensed() == true)
		{
			LOG_MSG(TEXT("Player is detected"));

			GetBlackboardComponent()->SetValueAsBool(BBKEY_ISDETECTED, _Stimulus.WasSuccessfullySensed());
			GetBlackboardComponent()->SetValueAsObject(BBKEY_TARGET, UpdatedPawn);
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
	//InitPerceptionSystem(_InPawn);
	RunAI();
}

void ABAIBasicMonsterController::InitPerceptionSystem(APawn* _InPawn)
{
	IBMonsterStatInterface* InPawnInterface = Cast<IBMonsterStatInterface>(_InPawn);
	if (InPawnInterface == nullptr)
	{
		LOG_MSG(TEXT("InPawn == nullptr"));
		return;
	}

	LOG_MSG(TEXT("PerceptionSystem is initted"));

	SightConfig->SightRadius = InPawnInterface->GetDetectRadius();
	SightConfig->PeripheralVisionAngleDegrees = 30.0f;

	//�־����� ������ �ʵ���. �ϴ��� ��� ���󰡰� ��.
	SightConfig->LoseSightRadius = InPawnInterface->GetDetectRadius();
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

		//�޼��� ������Ʈ�� � ������ ������ ���ΰ�.
		AIPerceptionComponent->ConfigureSense(*SightConfig);
		AIPerceptionComponent->SetDominantSense(*SightConfig->GetSenseImplementation());

		//�ش� ������Ʈ�� AI��Ʈ�ѷ��� ����.
		SetPerceptionComponent(*AIPerceptionComponent);

		//��������Ʈ�� ��Ȳ�� �´� �Լ��� ���ε�
		AIPerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &ABAIBasicMonsterController::OnTargetPerceptionUpdated);
		AIPerceptionComponent->OnTargetPerceptionForgotten.AddDynamic(this, &ABAIBasicMonsterController::OnTargetPerceptionForgotten);
	}
}
