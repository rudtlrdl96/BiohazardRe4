// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Player/Body/Notify/BAnimNotifyPMFootStepSound.h"
#include "Kismet/GameplayStatics.h"
#include "CollisionQueryParams.h"

UBAnimNotifyPMFootStepSound::UBAnimNotifyPMFootStepSound()
{
	static ConstructorHelpers::FObjectFinder<USoundWave> StaticDirtSound(TEXT("/Script/Engine.SoundWave'/Game/Assets/Player/Sound/FootStep/Dirt/SW_Leon_58.SW_Leon_58'"));
	DirtSound = StaticDirtSound;
	static ConstructorHelpers::FObjectFinder<USoundWave> StaticRockSound(TEXT("/Script/Engine.SoundWave'/Game/Assets/Player/Sound/FootStep/Rock/SW_Leon_87.SW_Leon_87'"));
	RockSound = StaticRockSound;
	static ConstructorHelpers::FObjectFinder<USoundWave> StaticWoodSound(TEXT("/Script/Engine.SoundWave'/Game/Assets/Player/Sound/FootStep/Wood/SW_Leon_59.SW_Leon_59'"));
	WoodSound = StaticWoodSound;
	static ConstructorHelpers::FObjectFinder<USoundWave> StaticWaterSound(TEXT("/Script/Engine.SoundWave'/Game/Assets/Player/Sound/FootStep/Water/SW_Leon_60.SW_Leon_60'"));
	WaterSound = StaticWaterSound;
	static ConstructorHelpers::FObjectFinder<USoundWave> StaticGrassSound(TEXT("/Script/Engine.SoundWave'/Game/Assets/Player/Sound/FootStep/Grass/SW_Leon_49.SW_Leon_49'"));
	GrassSound = StaticGrassSound;
}

void UBAnimNotifyPMFootStepSound::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	AActor* OwnerActor = MeshComp->GetOwner();

	FVector Location = OwnerActor->GetActorLocation();
	FHitResult HitResult;

	FCollisionQueryParams LineTraceParams;
	LineTraceParams.AddIgnoredActor(OwnerActor);
	LineTraceParams.bReturnPhysicalMaterial = true;

	if (true == OwnerActor->GetWorld()->LineTraceSingleByChannel(HitResult, Location, Location + FVector(0, 0, -150), ECollisionChannel::ECC_GameTraceChannel2, LineTraceParams))
	{
		if (nullptr == HitResult.PhysMaterial.Get())
		{
			return;
		}

		switch (HitResult.PhysMaterial.Get()->SurfaceType)
		{
		case EPhysicalSurface::SurfaceType1:// Dirt
		{
			UGameplayStatics::PlaySoundAtLocation(OwnerActor->GetWorld(), DirtSound.Object, HitResult.ImpactPoint);
		}
			break;
		case EPhysicalSurface::SurfaceType2:// Rock
		{
			UGameplayStatics::PlaySoundAtLocation(OwnerActor->GetWorld(), RockSound.Object, HitResult.ImpactPoint);
		}
			break;
		case EPhysicalSurface::SurfaceType3:// Wood
		{
			UGameplayStatics::PlaySoundAtLocation(OwnerActor->GetWorld(), WoodSound.Object, HitResult.ImpactPoint);
		}
			break;
		case EPhysicalSurface::SurfaceType4:// Water
		{
			UGameplayStatics::PlaySoundAtLocation(OwnerActor->GetWorld(), WaterSound.Object, HitResult.ImpactPoint);
		}
			break;
		case EPhysicalSurface::SurfaceType5:// Grass
		{
			UGameplayStatics::PlaySoundAtLocation(OwnerActor->GetWorld(), GrassSound.Object, HitResult.ImpactPoint);
		}
			break;
		default:
		{
			int a = 0;
		}
			break;
		}
	}
}