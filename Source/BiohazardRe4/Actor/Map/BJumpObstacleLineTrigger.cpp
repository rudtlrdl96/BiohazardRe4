// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Map/BJumpObstacleLineTrigger.h"
#include "Components/SceneComponent.h"
#include "Components/BoxComponent.h"

ABJumpObstacleLineTrigger::ABJumpObstacleLineTrigger()
{
	ObstacleWallTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("ObstacleWall"));
	ObstacleWallTrigger->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	ObstacleWallTrigger->SetRelativeLocation(FVector(FVector(0, 0, 50)));
	ObstacleWallTrigger->SetBoxExtent(FVector(1, 150, 100));
	ObstacleWallTrigger->SetCollisionProfileName("Obstacle");
}

FJumpData ABJumpObstacleLineTrigger::GetJumpMetaData(const FVector& _Location) const
{
	FJumpData Result = Super::GetJumpMetaData(_Location);
		
	FHitResult HitResult;

	bool bIsTarget = GetWorld()->LineTraceSingleByChannel(HitResult, _Location, _Location + (Result.End - Result.Start), ECollisionChannel::ECC_GameTraceChannel15);

	Result.bAbleJump = bIsTarget;

	if (false == bIsTarget)
	{
		return Result;
	}

	Result.Start = HitResult.ImpactPoint + (Result.Start - GetActorLocation());
	Result.End = HitResult.ImpactPoint + (Result.End - GetActorLocation());

	return Result;
}
