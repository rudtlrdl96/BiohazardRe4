// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Map/BMapBGMBase.h"

#include "BiohazardRe4.h"
#include "Sound/SoundCue.h"

#include "Components/AudioComponent.h"
#include "Sound/AudioVolume.h"
#include "Kismet/GameplayStatics.h"
// Sets default values
ABMapBGMBase::ABMapBGMBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	Sound_IdleBGM = CreateDefaultSubobject<UAudioComponent>(TEXT("BGMIDLESound"));
	Sound_SubIdleBGM = CreateDefaultSubobject<UAudioComponent>(TEXT("BGMIDELSUBSound"));
	Sound_HeightenBGM = CreateDefaultSubobject<UAudioComponent>(TEXT("BGMHEIGHENSound"));
	Sound_ChainsawBGM = CreateDefaultSubobject<UAudioComponent>(TEXT("BGMCHAINSAWSound"));
	SetSoundCue();
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ABMapBGMBase::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ABMapBGMBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABMapBGMBase::PlayIdleBGM()
{

	Sound_IdleBGM->Play();
	Sound_SubIdleBGM->Play();
	Sound_ChainsawBGM->FadeOut(1.0f,0.0f);
	Sound_HeightenBGM->FadeOut(1.0f, 0.0f);
}

void ABMapBGMBase::PlayHeightenSound()
{
	
	Sound_IdleBGM->FadeOut(1.0f, 0.0f);
	Sound_SubIdleBGM->FadeOut(1.0f, 0.0f);
	Sound_ChainsawBGM->FadeOut(1.0f, 0.0f);
	Sound_HeightenBGM->Play();


}

void ABMapBGMBase::PlayChainsawBGM()
{
	Sound_IdleBGM->FadeOut(1.0f, 0.0f);
	Sound_SubIdleBGM->FadeOut(1.0f, 0.0f);
	Sound_ChainsawBGM->Play();
	Sound_HeightenBGM->FadeOut(1.0f, 0.0f);
}

void ABMapBGMBase::BGMStop()
{
	Sound_IdleBGM->FadeOut(1.0f, 0.0f);
	Sound_SubIdleBGM->FadeOut(1.0f, 0.0f);
	Sound_ChainsawBGM->FadeOut(1.0f, 0.0f);
	Sound_HeightenBGM->FadeOut(1.0f, 0.0f);
}

void ABMapBGMBase::SetSoundCue()
{
	static ConstructorHelpers::FObjectFinder<USoundCue> IDLEBGMREF(TEXT("/Script/Engine.SoundCue'/Game/Assets/Map/Location01/Sound/SC_LevelIDLEBGM.SC_LevelIDLEBGM'"));
	if (IDLEBGMREF.Object != nullptr)
	{
		USoundCue* IDLEBGM = IDLEBGMREF.Object;

		Sound_IdleBGM->SetSound(IDLEBGM);
	}
	static ConstructorHelpers::FObjectFinder<USoundCue> SUBIDLEBGMREF(TEXT("/Script/Engine.SoundCue'/Game/Assets/Map/Location01/Sound/SC_LevelSubSound.SC_LevelSubSound'"));
	if (SUBIDLEBGMREF.Object != nullptr)
	{
		USoundCue* SUBIDLEBGM = SUBIDLEBGMREF.Object;

		Sound_SubIdleBGM->SetSound(SUBIDLEBGM);
	}

	static ConstructorHelpers::FObjectFinder<USoundCue> HEIGHTENBGMREF(TEXT("/Script/Engine.SoundCue'/Game/Assets/Map/Location01/Sound/SC_HeightenBGM_Cue.SC_HeightenBGM_Cue'"));
	if (HEIGHTENBGMREF.Object != nullptr)
	{
		USoundCue* HEIGHTENBGM = HEIGHTENBGMREF.Object;

		Sound_HeightenBGM->SetSound(HEIGHTENBGM);
	}

	static ConstructorHelpers::FObjectFinder<USoundCue> CHAINSAWBGMREF(TEXT("/Script/Engine.SoundCue'/Game/Assets/Map/Sound/SC_The_Chainsaw_Cue.SC_The_Chainsaw_Cue'"));
	if (CHAINSAWBGMREF.Object != nullptr)
	{
		USoundCue* CHAINSAWBGM = CHAINSAWBGMREF.Object;

		Sound_ChainsawBGM->SetSound(CHAINSAWBGM);
	}
}

