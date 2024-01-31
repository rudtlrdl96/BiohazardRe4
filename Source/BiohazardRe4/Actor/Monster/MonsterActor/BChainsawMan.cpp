// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Monster/MonsterActor/BChainsawMan.h"
#include "../AIController/BAIChainsawManController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "../Component/BMonsterStatComponent.h"
#include "../DataAsset/BMonsterStatData.h"
#include "BiohazardRe4.h"

ABChainsawMan::ABChainsawMan()
{
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> ChainsawSkeletalMeshRef(TEXT("/Script/Engine.SkeletalMesh'/Game/Assets/Monster/Mesh/ChainsawMan/Chaisaw/SK_Chainsaw.SK_Chainsaw'"));

	Weapon = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SK_Chainsaw"));
	Weapon->SetupAttachment(GetMesh(), TEXT("ChainsawSocket"));
	
	if (ChainsawSkeletalMeshRef.Object != nullptr)
	{
		Weapon->SetSkeletalMesh(ChainsawSkeletalMeshRef.Object);
		Weapon->SetCollisionProfileName(TEXT("NoCollision"));
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> ChainsawAttackMontageRef(TEXT("/Script/Engine.AnimMontage'/Game/Blueprints/Actor/Monster/Animation/AM_ChainsawManAttack.AM_ChainsawManAttack'"));
	
	if (ChainsawAttackMontageRef.Object != nullptr)
	{
		AttackMontage = ChainsawAttackMontageRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UBMonsterStatData> ChainsawManStatDataRef(TEXT("/Script/BiohazardRe4.BMonsterStatData'/Game/Blueprints/Actor/Monster/DataAsset/DA_ChainsawManStat.DA_ChainsawManStat'"));

	if (ChainsawManStatDataRef.Object != nullptr)
	{
		StatInit(ChainsawManStatDataRef.Object);
	}

	AIControllerClass = ABAIChainsawManController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorld;

	GetCharacterMovement()->MaxWalkSpeed = 100.0f;
	GetCharacterMovement()->bUseControllerDesiredRotation = true;
}

void ABChainsawMan::Attack()
{
	if (Weapon == nullptr)
	{
		LOG_WARNING(TEXT("Monster Weapon - Nullptr : ABMonsterBase::Attack()"));
		return;
	}

	Super::Attack();
}

void ABChainsawMan::BeginPlay()
{
	Super::BeginPlay();
}


