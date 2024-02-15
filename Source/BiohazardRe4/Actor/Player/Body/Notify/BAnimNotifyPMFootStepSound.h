// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "UObject/ConstructorHelpers.h"
#include "BAnimNotifyPMFootStepSound.generated.h"

class USoundWave;

/**
 * 
 */
UCLASS()
class BIOHAZARDRE4_API UBAnimNotifyPMFootStepSound : public UAnimNotify
{
	GENERATED_BODY()
public:
	UBAnimNotifyPMFootStepSound();

	void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference);

private:

	ConstructorHelpers::FObjectFinder<USoundWave> DirtSound = nullptr;
	ConstructorHelpers::FObjectFinder<USoundWave> RockSound = nullptr;
	ConstructorHelpers::FObjectFinder<USoundWave> WoodSound = nullptr;
	ConstructorHelpers::FObjectFinder<USoundWave> WaterSound = nullptr;
	ConstructorHelpers::FObjectFinder<USoundWave> GrassSound = nullptr;
};
