// Fill out your copyright notice in the Description page of Project Settings.


#include "BMonsterBase.h"

#include "Sound/SoundCue.h"
#include "Kismet/GameplayStatics.h"

void ABMonsterBase::PlaySound(ESoundType _PlaySoundType)
{
	if (SoundCues.Contains(_PlaySoundType) == true)
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), SoundCues[_PlaySoundType], GetActorLocation());
	}
}

bool ABMonsterBase::isAblePlay(ESoundType _PlaySoundType)
{
	if (_PlaySoundType == ESoundType::Yell)
	{
		if (isSetTargetInBlackBoard() == false)
		{
			return false;
		}
	}

	return true;
}
