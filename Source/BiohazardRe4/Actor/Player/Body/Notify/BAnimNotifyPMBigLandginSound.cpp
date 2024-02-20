// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Player/Body/Notify/BAnimNotifyPMBigLandginSound.h"
#include "Kismet/GameplayStatics.h"
#include "CollisionQueryParams.h"

UBAnimNotifyPMBigLandginSound::UBAnimNotifyPMBigLandginSound()
{
	static ConstructorHelpers::FObjectFinder<USoundCue> StaticDirtSound(TEXT("/Script/Engine.SoundCue'/Game/Blueprints/Actor/Player/Sound/Landing/Big/SC_LandingBig_Dirt.SC_LandingBig_Dirt'"));
	DirtSound = StaticDirtSound.Object;
	static ConstructorHelpers::FObjectFinder<USoundCue> StaticRockSound(TEXT("/Script/Engine.SoundCue'/Game/Blueprints/Actor/Player/Sound/Landing/Big/SC_LandingBig_Rock.SC_LandingBig_Rock'"));
	RockSound = StaticRockSound.Object;
	static ConstructorHelpers::FObjectFinder<USoundCue> StaticWoodSound(TEXT("/Script/Engine.SoundCue'/Game/Blueprints/Actor/Player/Sound/Landing/Big/SC_LandingBig_Wood.SC_LandingBig_Wood'"));
	WoodSound = StaticWoodSound.Object;
	static ConstructorHelpers::FObjectFinder<USoundCue> StaticWaterSound(TEXT("/Script/Engine.SoundCue'/Game/Blueprints/Actor/Player/Sound/Landing/Big/SC_LandingBig_Water.SC_LandingBig_Water'"));
	WaterSound = StaticWaterSound.Object;
	static ConstructorHelpers::FObjectFinder<USoundCue> StaticGrassSound(TEXT("/Script/Engine.SoundCue'/Game/Blueprints/Actor/Player/Sound/Landing/Big/SC_LandingBig_Grass.SC_LandingBig_Grass'"));
	GrassSound = StaticGrassSound.Object;
}

void UBAnimNotifyPMBigLandginSound::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
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
			UGameplayStatics::PlaySoundAtLocation(OwnerActor->GetWorld(), DirtSound, HitResult.ImpactPoint);
		}
		break;
		case EPhysicalSurface::SurfaceType2:// Rock
		{
			UGameplayStatics::PlaySoundAtLocation(OwnerActor->GetWorld(), RockSound, HitResult.ImpactPoint);
		}
		break;
		case EPhysicalSurface::SurfaceType3:// Wood
		{
			UGameplayStatics::PlaySoundAtLocation(OwnerActor->GetWorld(), WoodSound, HitResult.ImpactPoint);
		}
		break;
		case EPhysicalSurface::SurfaceType4:// Water
		{
			UGameplayStatics::PlaySoundAtLocation(OwnerActor->GetWorld(), WaterSound, HitResult.ImpactPoint);
		}
		break;
		case EPhysicalSurface::SurfaceType5:// Grass
		{
			UGameplayStatics::PlaySoundAtLocation(OwnerActor->GetWorld(), GrassSound, HitResult.ImpactPoint);
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
