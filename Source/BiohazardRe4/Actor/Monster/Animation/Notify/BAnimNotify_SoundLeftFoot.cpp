// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Monster/Animation/Notify/BAnimNotify_SoundLeftFoot.h"
#include "Actor/Monster/Interface/BMonsterSoundInterface.h"

void UBAnimNotify_SoundLeftFoot::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
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

	FVector Location = Owner->GetActorLocation();
	FHitResult HitResult;

	FCollisionQueryParams LineTraceParams;
	LineTraceParams.AddIgnoredActor(Owner);
	LineTraceParams.bReturnPhysicalMaterial = true;

	if (true == Owner->GetWorld()->LineTraceSingleByChannel(HitResult, Location + FVector(0.0f, 0.0f, 5.0f), Location + FVector(0, 0, -150), ECollisionChannel::ECC_GameTraceChannel2, LineTraceParams))
	{
		if (nullptr == HitResult.PhysMaterial.Get())
		{
			return;
		}

		switch (HitResult.PhysMaterial.Get()->SurfaceType)
		{
		case EPhysicalSurface::SurfaceType1:
			SoundInterface->PlayFootSound(EFootSoundType::Dirt, true);
		break;

		case EPhysicalSurface::SurfaceType2:
			SoundInterface->PlayFootSound(EFootSoundType::Rock, true);
		break;

		case EPhysicalSurface::SurfaceType3:
			SoundInterface->PlayFootSound(EFootSoundType::Wood, true);
		break;

		case EPhysicalSurface::SurfaceType4:
			SoundInterface->PlayFootSound(EFootSoundType::Water, true);
		break;

		case EPhysicalSurface::SurfaceType5:
			SoundInterface->PlayFootSound(EFootSoundType::Grass, true);
		break;

		default:
			break;
		}
	}
}
