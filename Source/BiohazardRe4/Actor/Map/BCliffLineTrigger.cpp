// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Map/BCliffLineTrigger.h"

FJumpData ABCliffLineTrigger::GetJumpMetaData(const FVector& _Location) const
{
	FJumpData Result = Super::GetJumpMetaData(_Location);

	FHitResult HitResult;

	if (true == GetWorld()->LineTraceSingleByChannel(HitResult, _Location, _Location + FVector(0, 0, -500), ECollisionChannel::ECC_GameTraceChannel2))
	{
		Result.End = HitResult.ImpactPoint;
	}
	else
	{
		Result.End += FVector(0, 0, -500);
	}

	return Result;
}
