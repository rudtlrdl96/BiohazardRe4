// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Monster/BMonsterBase.h"
#include "Component/BMonsterStatComponent.h"
#include "BiohazardRe4.h"
#include "Components/CapsuleComponent.h"

// Sets default values
ABMonsterBase::ABMonsterBase()
{
	PrimaryActorTick.bCanEverTick = true;
	bUseControllerRotationYaw = false;

	Stat = CreateDefaultSubobject<UBMonsterStatComponent>(TEXT("Stat"));
}

void ABMonsterBase::BeginPlay()
{
	Super::BeginPlay();
}

MonsterState ABMonsterBase::GetCurrentState()
{
	return CurState;
}

void ABMonsterBase::SetCurrentState(MonsterState _InState)
{
	CurState = _InState;
}

void ABMonsterBase::Attack()
{
	if (Weapon == nullptr)
	{
		LOG_WARNING(TEXT("Monster Weapon - Nullptr : ABMonsterBase::Attack()"));
		return;
	}

	float AttackRadius = GetWeaponAttackRadius();
	float AttackRange = GetAttackRange();

	const FVector Start = GetActorLocation() + GetActorForwardVector() * GetCapsuleComponent()->GetScaledCapsuleRadius();
	const FVector End = Start + GetActorForwardVector() * AttackRange;
	
	FHitResult OutHitResult;
	FCollisionQueryParams Params(SCENE_QUERY_STAT(Attack), false, this);

	bool HitDetected = GetWorld()->SweepSingleByChannel(OutHitResult, Start, End, FQuat::Identity, ECC_EngineTraceChannel2, FCollisionShape::MakeSphere(AttackRadius), Params);

#if ENABLE_DRAW_DEBUG
	
	FColor DebugColor = FColor::Green;
	if (HitDetected == true)
	{
		DebugColor = FColor::Red;
	}

	FVector CapsuleOrigin = Start + (End - Start) * 0.5f;
	float CapsuleHalfHeight = AttackRange * 0.5f;

	DrawDebugCapsule(GetWorld(), CapsuleOrigin, CapsuleHalfHeight, AttackRadius, FRotationMatrix::MakeFromZ(GetActorForwardVector()).ToQuat(), DebugColor, false, 5.0f);
#endif
}

bool ABMonsterBase::IsAttacking()
{
	return bIsAttacking;
}

void ABMonsterBase::SetIsAttack(bool _IsAttacking)
{
	bIsAttacking = _IsAttacking;
}

void ABMonsterBase::SetMonsterAttackEndDelegate(FMonsterAttackEnd& _InAttackEnd)
{
	OnAttackEnd = _InAttackEnd;
}

const FMonsterAttackEnd& ABMonsterBase::GetMonsterAttackEndDelegate()
{
	return OnAttackEnd;
}

float ABMonsterBase::GetAttackRange()
{
	return Stat->GetAttackRange();
}

float ABMonsterBase::GetWeaponAttackRadius()
{
	return Stat->GetWeaponAttackRadius();
}

