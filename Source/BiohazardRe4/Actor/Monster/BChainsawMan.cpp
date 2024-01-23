// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Monster/BChainsawMan.h"
#include "AIController/BAIChainsawManController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Component/BMonsterStatComponent.h"
#include "BiohazardRe4.h"

ABChainsawMan::ABChainsawMan()
{
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> ChainsawSkeletalMeshRef(TEXT("/Script/Engine.SkeletalMesh'/Game/Assets/Monster/Mesh/Chainsaw/SK_Chainsaw.SK_Chainsaw'"));

	Weapon = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SK_Chainsaw"));
	Weapon->SetupAttachment(GetMesh(), TEXT("ChainsawSocket"));
	//
	if (ChainsawSkeletalMeshRef.Object != nullptr)
	{
		Weapon->SetSkeletalMesh(ChainsawSkeletalMeshRef.Object);
		Weapon->SetCollisionProfileName(TEXT("NoCollision"));
	}

	AIControllerClass = ABAIChainsawManController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorld;

	GetCharacterMovement()->MaxWalkSpeed = 100.0f;
	GetCharacterMovement()->bUseControllerDesiredRotation = true;

	Stat->SetAttackRadius(120);
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

