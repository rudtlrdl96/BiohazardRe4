// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Player/Weapon/Gun/BEmptyShell.h"
#include "BiohazardRe4.h"

ABEmptyShell::ABEmptyShell()
{
	GetStaticMeshComponent()->Mobility = EComponentMobility::Movable;
	GetStaticMeshComponent()->SetSimulatePhysics(true);
}

void ABEmptyShell::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	LOG_MSG(TEXT("ABEmptyShellTick"))
	Time += DeltaTime;
	if (Time >= LifeTime)
	{
		Destroy();
	}
}
