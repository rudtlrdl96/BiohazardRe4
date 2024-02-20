// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "BWAnimNotifySoundMagazine2.generated.h"

/**
 * 
 */
UCLASS()
class BIOHAZARDRE4_API UBWAnimNotifySoundMagazine2 : public UAnimNotify
{
	GENERATED_BODY()
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

};
