// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Monster/BehaviorTree/Common/BBTService_SpeedUpdate.h"
#include "../../Interface/BMonsterStateInterface.h"
#include "AIController.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "../../Define/MonsterDefine.h"
#include "BiohazardRe4.h"

void UBBTService_SpeedUpdate::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	APawn* MyPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (MyPawn == nullptr)
	{
		LOG_FATAL(TEXT("MyPawn == nullptr : UBTService_CheckDistanceToTarget::TickNode"));
	}

	ACharacter* MyCharacter = OwnerComp.GetAIOwner()->GetCharacter();
	if (MyCharacter == nullptr)
	{
		LOG_FATAL(TEXT("MyCharacter == nullptr : UBTService_CheckDistanceToTarget::TickNode"));
	}

	UWorld* CurWorld = MyPawn->GetWorld();
	if (CurWorld == nullptr)
	{
		LOG_FATAL(TEXT("CurWorld == nullptr : UBTService_CheckDistanceToTarget::TickNode"));
	}

	APlayerController* TargetController = CurWorld->GetFirstPlayerController();
	if (TargetController == nullptr)
	{
		LOG_FATAL(TEXT("TargetController == nullptr : UBTService_CheckDistanceToTarget::TickNode"));
	}

	ACharacter* TargetCharacter = TargetController->GetCharacter();
	if (TargetCharacter == nullptr)
	{
		LOG_FATAL(TEXT("TargetCharacter == nullptr : UBTService_CheckDistanceToTarget::TickNode"));
	}

	float DistanceToTarget = MyCharacter->GetDistanceTo(TargetCharacter);
	float DistanceThreshold = 600.0f;

	//몬스터와 플레이어간의 거리 (뛸 거냐 걸을 거냐)
	if (DistanceToTarget > DistanceThreshold)
	{
		MyCharacter->GetCharacterMovement()->MaxWalkSpeed = 100.0f;
	}
	else if (DistanceToTarget <= DistanceThreshold)
	{
		MyCharacter->GetCharacterMovement()->MaxWalkSpeed = 50.0f;
	}
}
