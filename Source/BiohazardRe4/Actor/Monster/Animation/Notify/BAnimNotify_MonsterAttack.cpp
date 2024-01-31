// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Monster/Animation/Notify/BAnimNotify_MonsterAttack.h"
#include "../../Interface/BMonsterStateInterface.h"
#include "BiohazardRe4.h"
#include "GameFramework/Character.h"

#include "../../Interface/BMonsterStatInterface.h"

void UBAnimNotify_MonsterAttack::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (MeshComp != nullptr)
	{
		ACharacter* Owner = Cast<ACharacter>(MeshComp->GetOwner());
		if (Owner == nullptr)
		{
			LOG_WARNING(TEXT("Owner == nullptr : UBAnimNotify_MonsterAttack::Notify"));
			return;
		}

		IBMonsterStateInterface* Interface = Cast<IBMonsterStateInterface>(Owner);
		if (Interface == nullptr)
		{
			LOG_WARNING(TEXT("Interface == nullptr : UBAnimNotify_MonsterAttack::Notify"));
			return;
		}
		
		Interface->Attack();
	}
}
