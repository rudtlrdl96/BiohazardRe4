// Fill out your copyright notice in the Description page of Project Settings.


#include "BMonsterBase.h"

#include "Sound/SoundCue.h"
#include "Kismet/GameplayStatics.h"
#include "Components/AudioComponent.h"

#include "BiohazardRe4.h"

void ABMonsterBase::PlaySound(ESoundType _PlaySoundType)
{
	if (isAblePlay(_PlaySoundType) == true)
	{
		GeneralSoundComp->SetSound(GeneralSoundCues[_PlaySoundType]);
		GeneralSoundComp->Play();
	}
}

bool ABMonsterBase::isAblePlay(ESoundType _PlaySoundType)
{
	if (GeneralSoundCues.Contains(_PlaySoundType) == false)
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

void ABMonsterBase::PlayFootSound(EFootSoundType _PlaySoundType, bool _isLeft)
{
	if (_isLeft == true)
	{
		LeftFootSoundComp->SetSound(FootSoundCues[_PlaySoundType]);
		LeftFootSoundComp->Play();
	}
	else
	{
		RightFootSoundComp->SetSound(FootSoundCues[_PlaySoundType]);
		RightFootSoundComp->Play();
	}
}

void ABMonsterBase::LoadFootSoundCue()
{
	static ConstructorHelpers::FObjectFinder<USoundCue> FootDirtRef(TEXT("/Script/Engine.SoundCue'/Game/Blueprints/Actor/Monster/SoundCue/FootSoundCue/SC_FootDirtCue.SC_FootDirtCue'"));
	if (FootDirtRef.Object != nullptr)
	{
		USoundCue* FootDirtCue = FootDirtRef.Object;
		FootSoundCues.Add(EFootSoundType::Dirt, FootDirtCue);
	}

	static ConstructorHelpers::FObjectFinder<USoundCue> FootRockRef(TEXT("/Script/Engine.SoundCue'/Game/Blueprints/Actor/Monster/SoundCue/FootSoundCue/SC_FootRockCue.SC_FootRockCue'"));
	if (FootRockRef.Object != nullptr)
	{
		USoundCue* FootRockCue = FootRockRef.Object;
		FootSoundCues.Add(EFootSoundType::Rock, FootRockCue);
	}

	static ConstructorHelpers::FObjectFinder<USoundCue> FootWaterRef(TEXT("/Script/Engine.SoundCue'/Game/Blueprints/Actor/Monster/SoundCue/FootSoundCue/SC_FootWaterCue.SC_FootWaterCue'"));
	if (FootWaterRef.Object != nullptr)
	{
		USoundCue* FootWaterCue = FootWaterRef.Object;
		FootSoundCues.Add(EFootSoundType::Water, FootWaterCue);
	}

	static ConstructorHelpers::FObjectFinder<USoundCue> FootGrassRef(TEXT("/Script/Engine.SoundCue'/Game/Blueprints/Actor/Monster/SoundCue/FootSoundCue/SC_FootGrassCue.SC_FootGrassCue'"));
	if (FootGrassRef.Object != nullptr)
	{
		USoundCue* FootGrassCue = FootGrassRef.Object;
		FootSoundCues.Add(EFootSoundType::Grass, FootGrassCue);
	}

	static ConstructorHelpers::FObjectFinder<USoundCue> FootWoodRef(TEXT("/Script/Engine.SoundCue'/Game/Blueprints/Actor/Monster/SoundCue/FootSoundCue/SC_FootWoodCue.SC_FootWoodCue'"));
	if (FootWoodRef.Object != nullptr)
	{
		USoundCue* FootWoodCue = FootWoodRef.Object;
		FootSoundCues.Add(EFootSoundType::Wood, FootWoodCue);
	}
}
