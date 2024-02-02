// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Player/Body/BAnimNotifyKnifeAttackStart.h"
#include "BiohazardRe4.h"
#include "BInterface_KnifeAttack.h"
#include "GameFramework/Character.h"

void UBAnimNotifyKnifeAttackStart::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (MeshComp != nullptr)
	{
		ACharacter* Owner = Cast<ACharacter>(MeshComp->GetOwner());
		if (Owner == nullptr)
		{
			return;
		}

		IBInterface_KnifeAttack* Interface = Cast<IBInterface_KnifeAttack>(Owner);

		if (Interface == nullptr)
		{
			LOG_FATAL(TEXT("Interface Casting Fail"));
		}

		Interface->KnifeAttackStart();
	}
}