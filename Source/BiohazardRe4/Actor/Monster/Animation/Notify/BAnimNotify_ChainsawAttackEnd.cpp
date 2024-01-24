// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Monster/Animation/Notify/BAnimNotify_ChainsawAttackEnd.h"
#include "../../Interface/BMonsterStateInterface.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "BiohazardRe4.h"

void UBAnimNotify_ChainsawAttackEnd::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	if (MeshComp == nullptr)
	{
		LOG_WARNING(TEXT("MeshComp == nullptr : UBAnimNotify_ChainsawAttackEnd::Notify - Parameter"));
		return;
	}
	Super::Notify(MeshComp, Animation, EventReference);

	if (MeshComp != nullptr)
	{
		IBMonsterStateInterface* StateInterface = Cast<IBMonsterStateInterface>(MeshComp->GetOwner());

		if (StateInterface != nullptr)
		{
			StateInterface->SetCurrentState(MonsterState::Walk);
			
			UWorld* CurWorld = MeshComp->GetWorld();
			StateInterface->GetMonsterAttackEndDelegate().ExecuteIfBound();
		}
	}

	ACharacter* MeshCharacter = Cast<ACharacter>(MeshComp->GetOwner());
	if (MeshCharacter == nullptr)
	{
		LOG_WARNING(TEXT("MeshCharacter is Nullptr : UBAnimNotify_ChainsawAttackEnd::Notify - 30"));
		return;
	}

	//MeshCharacter->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
}
