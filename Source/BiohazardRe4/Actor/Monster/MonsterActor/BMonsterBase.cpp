// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Monster/MonsterActor/BMonsterBase.h"
#include "../Component/BMonsterStatComponent.h"
#include "BiohazardRe4.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
ABMonsterBase::ABMonsterBase()
{
	PrimaryActorTick.bCanEverTick = false;
	bUseControllerRotationYaw = false;

	Stat = CreateDefaultSubobject<UBMonsterStatComponent>(TEXT("Stat"));
}

void ABMonsterBase::BeginPlay()
{
	Super::BeginPlay();
}

void ABMonsterBase::AttackStart()
{
	if (AttackMontage == nullptr)
	{
		LOG_WARNING(TEXT("AttackMontage is Nullptr"));
		return;
	}

	int SectionNumber = FMath::RandRange(1, 2);
	FName SectionName = *FString::Printf(TEXT("Attack%d"), SectionNumber);

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	if (AnimInstance == nullptr)
	{
		LOG_WARNING(TEXT("AnimInstance is Nullptr"));
		return;
	}

	AnimInstance->Montage_Play(AttackMontage, 1.0f/*공속이 있다면, 바꿔줘야겠지?*/);
	AnimInstance->Montage_JumpToSection(SectionName, AttackMontage);

	AnimInstance->RootMotionMode = ERootMotionMode::RootMotionFromEverything;
}

void ABMonsterBase::Attack()
{
	float SweepRadius = GetAttackSweepRadius();
	float AttackRange = GetAttackRadius();

	const FVector Start = GetActorLocation() + GetActorForwardVector() * GetCapsuleComponent()->GetScaledCapsuleRadius();
	const FVector End = Start + GetActorForwardVector() * AttackRange;
	
	FHitResult OutHitResult;
	FCollisionQueryParams Params(SCENE_QUERY_STAT(Attack), false, this);

	bool HitDetected = GetWorld()->SweepSingleByChannel(
		                           OutHitResult, 
		                           Start, End, 
		                           FQuat::Identity, 
		                           ECC_EngineTraceChannel2, 
		                           FCollisionShape::MakeSphere(SweepRadius), Params);
	
	
	AActor* HitActor = OutHitResult.GetActor();
	ACharacter* HitCharacter = Cast<ACharacter>(HitActor);

	if (HitCharacter == nullptr)
	{
		LOG_WARNING(TEXT("Type Casting failed : HitCharacter"));
	}

	AController* HitController = nullptr;
	if (HitCharacter != nullptr)
	{
		HitController = HitCharacter->GetController();
		if (HitController == nullptr)
		{
			LOG_WARNING(TEXT("HitController가 nullptr입니다."));
		}
	}

	if (HitController != nullptr && HitController->IsPlayerController() == true)
	{
		//때린다.
	}

#if ENABLE_DRAW_DEBUG
	
	FColor DebugColor = FColor::Green;

	if (HitController != nullptr && HitController->IsPlayerController() == true)
	{
		DebugColor = FColor::Red;
	}

	FVector CapsuleOrigin = Start + (End - Start) * 0.5f;
	float CapsuleHalfHeight = AttackRange * 0.5f;

	DrawDebugCapsule(GetWorld(), CapsuleOrigin, CapsuleHalfHeight, SweepRadius, FRotationMatrix::MakeFromZ(GetActorForwardVector()).ToQuat(), DebugColor, false, 2.5f);
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

float ABMonsterBase::GetAttackRadius()
{
	return Stat->GetAttackRadius();
}

float ABMonsterBase::GetAttackSweepRadius()
{
	return Stat->GetAttackSweepRadius();
}

float ABMonsterBase::GetDetectRadius()
{
	return Stat->GetDetectRadius();
}

float ABMonsterBase::GetPatrolRadius()
{
	return Stat->GetPatrolRadius();
}

EMonsterState ABMonsterBase::GetCurrentState()
{
	return CurState;
}

void ABMonsterBase::SetCurrentState(EMonsterState _InState)
{
	CurState = _InState;

	if (_InState == EMonsterState::Attack)
	{
		AttackStart();
	}
}
