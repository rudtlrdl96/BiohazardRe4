// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "BAnimNotify_CrossWindowJumpStart.generated.h"

/**
 * 
 */
UCLASS()
class BIOHAZARDRE4_API UBAnimNotify_CrossWindowJumpStart : public UAnimNotify
{
	GENERATED_BODY()

public:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
};