// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "UObject/ConstructorHelpers.h"
#include "Sound/SoundCue.h"
#include "BAnimNotifyPMSmallLandginSound.generated.h"

class USoundWave;

/**
 * 
 */
UCLASS()
class BIOHAZARDRE4_API UBAnimNotifyPMSmallLandginSound : public UAnimNotify
{
	GENERATED_BODY()
public:
	UBAnimNotifyPMSmallLandginSound();

	void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference);

private:

	USoundCue* DirtSound = nullptr;
	USoundCue* RockSound = nullptr;
	USoundCue* WoodSound = nullptr;
	USoundCue* WaterSound = nullptr;
	USoundCue* GrassSound = nullptr;
};
