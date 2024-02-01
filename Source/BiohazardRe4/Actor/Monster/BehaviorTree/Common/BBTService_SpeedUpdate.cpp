// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Monster/BehaviorTree/Common/BBTService_SpeedUpdate.h"
#include "../../Interface/BMonsterStatInterface.h"
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
		LOG_FATAL(TEXT("MyPawn is nullptr"));
		return;
	}

	ACharacter* MyCharacter = OwnerComp.GetAIOwner()->GetCharacter();
	if (MyCharacter == nullptr)
	{
		LOG_FATAL(TEXT("MyCharacter is nullptr"));
		return;
	}

	UWorld* CurWorld = MyPawn->GetWorld();
	if (CurWorld == nullptr)
	{
		LOG_FATAL(TEXT("CurWorld is nullptr"));
		return;
	}

	APlayerController* TargetController = CurWorld->GetFirstPlayerController();
	if (TargetController == nullptr)
	{
		LOG_FATAL(TEXT("TargetController is nullptr"));
		return;
	}

	ACharacter* TargetCharacter = TargetController->GetCharacter();
	if (TargetCharacter == nullptr)
	{
		LOG_FATAL(TEXT("TargetCharacter is nullptr"));
		return;
	}

	IBMonsterStatInterface* Interface = Cast<IBMonsterStatInterface>(MyCharacter);
	if (Interface == nullptr)
	{
		LOG_FATAL(TEXT("Interface is nullptr"));
		return;
	}

	float DistanceToTarget = MyCharacter->GetDistanceTo(TargetCharacter);
	float DistanceThreshold = Interface->GetWalkDistanceThreshold();

	//몬스터와 플레이어간의 거리 (뛸 거냐 걸을 거냐)
	if (DistanceToTarget > DistanceThreshold)
	{
		MyCharacter->GetCharacterMovement()->MaxWalkSpeed = Interface->GetRunSpeed();
	}
	else if (DistanceToTarget <= DistanceThreshold)
	{
		MyCharacter->GetCharacterMovement()->MaxWalkSpeed = Interface->GetWalkSpeed();
	}
}
