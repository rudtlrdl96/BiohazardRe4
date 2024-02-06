// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Monster/MonsterActor/BDummyMonster.h"

ABDummyMonster::ABDummyMonster()
{
	AutoPossessAI = EAutoPossessAI::Disabled;
	SetCurrentState(EMonsterState::Groggy);


	static ConstructorHelpers::FObjectFinder<USkeletalMesh> MeshRef(TEXT("/Script/Engine.SkeletalMesh'/Game/Assets/Monster/Mesh/BasicMonster/Male/Base/SK_MonsterMaleBase.SK_MonsterMaleBase'"));
	if (MeshRef.Object != nullptr)
	{
		GetMesh()->SetSkeletalMesh(MeshRef.Object);
	}
}
