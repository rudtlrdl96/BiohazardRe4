// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Monster/Animation/Notify/BAnimNotify_SoundYellAtPlayer.h"
#include "BiohazardRe4.h"
#include "Sound/SoundCue.h"
#include "Kismet/GameplayStatics.h"

#include "Actor/Monster/Interface/BMonsterSoundInterface.h"

void UBAnimNotify_SoundYellAtPlayer::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	if (MeshComp == nullptr)
	{
		return;
	}

	AActor* Owner = MeshComp->GetOwner();
	if (Owner == nullptr)
	{
		return;
	}

	IBMonsterSoundInterface* SoundInterface = Cast<IBMonsterSoundInterface>(Owner);
	if (SoundInterface == nullptr)
	{
		return;
	}
		
	if (SoundInterface->isAblePlay(ESoundType::Yell) == true)
	{
		SoundInterface->PlaySound(ESoundType::Yell);
	}
}
