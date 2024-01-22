// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Monster/BehaviorTree/BBTService_SpeedUpdate.h"
#include "../Interface/BMonsterStateInterface.h"
#include "AIController.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "../Define/MonsterDefine.h"
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
	float DeltaTime = CurWorld->GetDeltaSeconds();

	//���Ϳ� �÷��̾�� �Ÿ� (�� �ų� ���� �ų�)
	if (DistanceToTarget > DistanceThreshold)
	{
		float CurSpeed = MyCharacter->GetCharacterMovement()->MaxWalkSpeed;
		float NextSpeed = FMath::Lerp(CurSpeed, 100, 0.5f);

		NextSpeed = FMath::Min(100, NextSpeed);
		MyCharacter->GetCharacterMovement()->MaxWalkSpeed = NextSpeed;
	}
	else if (DistanceToTarget <= DistanceThreshold)
	{
		float CurSpeed = MyCharacter->GetCharacterMovement()->MaxWalkSpeed;
		float NextSpeed = FMath::Lerp(CurSpeed, 50, 0.5f);

		NextSpeed = FMath::Max(50, NextSpeed);
		MyCharacter->GetCharacterMovement()->MaxWalkSpeed = NextSpeed;
	}
}
