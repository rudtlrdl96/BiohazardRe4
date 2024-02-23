// Fill out your copyright notice in the Description page of Project Settings.


#include "BAnimNotify_MonsterDamagedEnd.h"

#include "GameFramework/Character.h"

#include "BiohazardRe4.h"
#include "Actor/Monster/Interface/BMonsterStateInterface.h"

void UBAnimNotify_MonsterDamagedEnd::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (MeshComp != nullptr)
	{
		ACharacter* Owner = Cast<ACharacter>(MeshComp->GetOwner());
		if (Owner == nullptr)
		{
			//LOG_WARNING(TEXT("Owner is nullptr"));
			return;
		}

		IBMonsterStateInterface* Interface = Cast<IBMonsterStateInterface>(Owner);
		if (Interface == nullptr)
		{
			//LOG_WARNING(TEXT("Interface is nullptr"));
			return;
		}

		Interface->DamagedEnd();
	}
}
