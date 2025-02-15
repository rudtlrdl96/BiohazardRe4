// Fill out your copyright notice in the Description page of Project Settings.


#include "BAnimNotify_MonsterAttackEnd.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "BiohazardRe4.h"
#include "Actor/Monster/Interface/BMonsterStateInterface.h"

void UBAnimNotify_MonsterAttackEnd::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (MeshComp == nullptr)
	{
		//LOG_WARNING(TEXT("MeshComp is Nullptr"));
		return;
	}

	Super::Notify(MeshComp, Animation, EventReference);

	if (MeshComp != nullptr)
	{
		IBMonsterStateInterface* StateInterface = Cast<IBMonsterStateInterface>(MeshComp->GetOwner());

		if (StateInterface != nullptr)
		{
			StateInterface->SetCurrentState(EMonsterState::Walk);
			//LOG_MSG(TEXT("StateChanged - Walk"));
			UWorld* CurWorld = MeshComp->GetWorld();
			StateInterface->GetMonsterAttackEndDelegate().ExecuteIfBound();
		}
	}

	ACharacter* MeshCharacter = Cast<ACharacter>(MeshComp->GetOwner());
	if (MeshCharacter == nullptr)
	{
		//LOG_WARNING(TEXT("MeshCharacter is Nullptr : UBAnimNotify_ChainsawAttackEnd::Notify - 30"));
		return;
	}
}
