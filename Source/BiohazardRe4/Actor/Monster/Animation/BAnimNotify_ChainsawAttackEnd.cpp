// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Monster/Animation/BAnimNotify_ChainsawAttackEnd.h"
#include "../Interface/BMonsterStateInterface.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

void UBAnimNotify_ChainsawAttackEnd::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (MeshComp != nullptr)
	{
		IBMonsterStateInterface* StateInterface = Cast<IBMonsterStateInterface>(MeshComp->GetOwner());

		if (StateInterface != nullptr)
		{
			StateInterface->SetCurrentState(MonsterState::Walk);
			StateInterface->SetIsAttack(false);
		}
	}

	ACharacter* MeshCharacter = Cast<ACharacter>(MeshComp->GetOwner());
	if (MeshCharacter != nullptr)
	{
		MeshCharacter->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
		UE_LOG(LogTemp, Log, TEXT("MeshCharacter is Nullptr : UBAnimNotify_ChainsawAttackEnd::Notify - 28"));
	}
}
