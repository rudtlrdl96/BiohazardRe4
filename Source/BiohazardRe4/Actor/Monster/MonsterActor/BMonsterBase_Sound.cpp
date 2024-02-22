// Fill out your copyright notice in the Description page of Project Settings.


#include "BMonsterBase.h"

#include "Sound/SoundCue.h"
#include "Kismet/GameplayStatics.h"
#include "Components/AudioComponent.h"

void ABMonsterBase::PlaySound(ESoundType _PlaySoundType)
{
	if (isAblePlay(_PlaySoundType) == true)
	{
		SoundComponent->SetSound(SoundCues[_PlaySoundType]);
		SoundComponent->Play();
	}
}

bool ABMonsterBase::isAblePlay(ESoundType _PlaySoundType)
{
	if (SoundCues.Contains(_PlaySoundType) == false)
	{
		return false;
	}

	switch (_PlaySoundType)
	{
	case ESoundType::Idle:
		break;
	case ESoundType::Mutter:
		if (isSetTargetInBlackBoard() == false ||
			GetCurrentState() != EMonsterState::Walk)
		{
			return false;
		}
		break;
	case ESoundType::Yell:
		if (isSetTargetInBlackBoard() == false ||
			GetCurrentState() != EMonsterState::Run)
		{
			return false;
		}
		break;
	case ESoundType::Detect:
		break;

	case ESoundType::Damaged:
		break;

	case ESoundType::GeneralGroggy:
		break;

	case ESoundType::HeadShotGroggy:
		break;

	case ESoundType::PointDeath:
		break;
	default:

		break;
	}

	return true;
}
