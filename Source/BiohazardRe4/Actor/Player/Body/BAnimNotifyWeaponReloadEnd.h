// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "BAnimNotifyWeaponReloadEnd.generated.h"

/**
 * 
 */
UCLASS()
class BIOHAZARDRE4_API UBAnimNotifyWeaponReloadEnd : public UAnimNotify
{
	GENERATED_BODY()

public:
	void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference);
};
