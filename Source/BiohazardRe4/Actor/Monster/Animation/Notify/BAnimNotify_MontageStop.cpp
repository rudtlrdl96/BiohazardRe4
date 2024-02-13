// Fill out your copyright notice in the Description page of Project Settings.


#include "BAnimNotify_MontageStop.h"
#include "Actor/Monster/Interface/BMonsterStateInterface.h"

void UBAnimNotify_MontageStop::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	AActor* Actor = MeshComp->GetOwner();
	if (Actor == nullptr)
	{
		return;
	}

	IBMonsterStateInterface* Interface = Cast<IBMonsterStateInterface>(Actor);
	if (Interface == nullptr)
	{
		return;
	}
	
	EMonsterState CurState = Interface->GetCurrentState();
	if (CurState != EMonsterState::Death)
	{
		return;
	}

	UAnimInstance* AnimInstance = MeshComp->GetAnimInstance();
	if (AnimInstance != nullptr)
	{
		AnimInstance->Montage_Pause(AnimInstance->GetCurrentActiveMontage());
	}
}
