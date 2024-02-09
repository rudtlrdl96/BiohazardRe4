// Fill out your copyright notice in the Description page of Project Settings.


#include "BAnimNotify_MontageStop.h"

void UBAnimNotify_MontageStop::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{

	UAnimInstance* AnimInstance = MeshComp->GetAnimInstance();
	if (AnimInstance != nullptr)
	{
		AnimInstance->Montage_Pause(AnimInstance->GetCurrentActiveMontage());
	}
}
