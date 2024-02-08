// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Monster/MonsterActor/BMonsterBase.h"
#include "../Component/BMonsterStatComponent.h"
#include "BiohazardRe4.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "../Define/MonsterDefine.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/DamageEvents.h"
#include "../DataAsset/BMonsterStatData.h"
#include "PhysicsEngine/PhysicsAsset.h"

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

	SetDamagedSectionNums();
}

void ABMonsterBase::AttackStart()
{
	if (AttackMontage == nullptr)
	{
		LOG_WARNING(TEXT("AttackMontage is Nullptr"));
		return;
	}

	SetCurrentState(EMonsterState::Attack);

	int SectionNumber = FMath::RandRange(1, DamageTypes.Num());
	FName SectionName = *FString::Printf(TEXT("Attack%d"), SectionNumber);

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	if (AnimInstance == nullptr)
	{
		LOG_WARNING(TEXT("AnimInstance is Nullptr"));
		return;
	}

	AnimInstance->Montage_Play(AttackMontage, 1.0f/*공속이 있다면, 바꿔줘야겠지?*/);
	AnimInstance->Montage_JumpToSection(SectionName, AttackMontage);
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
		LOG_MSG(TEXT("Monster Attack To %s"), *HitCharacter->GetName());

		HitController = HitCharacter->GetController();
		if (HitController == nullptr)
		{
			LOG_WARNING(TEXT("HitController is nullptr"));
		}
	}

	if (HitCharacter != nullptr && HitController != nullptr && HitController->IsPlayerController() == true)
	{
		FName CurSection = GetMesh()->GetAnimInstance()->Montage_GetCurrentSection();
		FString CurSectionStr = CurSection.ToString();

		FString CurSectionNumberStr = CurSectionStr.Mid(CurSectionStr.Len() - 1, 1);
		int CurSectionNumberInt = FCString::Atoi(*CurSectionNumberStr);

		if (DamageTypes.IsValidIndex(CurSectionNumberInt - 1) == false)
		{
			LOG_FATAL(TEXT("CurSectionNumber is invalid : %d , %s"), CurSectionNumberInt, *CurSectionStr);
			return;
		}

		UGameplayStatics::ApplyDamage(HitCharacter, Stat->GetBaseAttackPower(), GetController(), this, DamageTypes[CurSectionNumberInt - 1]);
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

float ABMonsterBase::GetRunSpeed()
{
	return Stat->GetRunSpeed();
}

float ABMonsterBase::GetWalkSpeed()
{
	return Stat->GetWalkSpeed();
}

float ABMonsterBase::GetWalkDistanceThreshold()
{
	return Stat->GetWalkDistanceThreshold();
}

void ABMonsterBase::StatInit(const UBMonsterStatData* _DataAsset)
{
	FStatStruct StatStruct;

	StatStruct.MaxHp = _DataAsset->MaxHp;
	StatStruct.CurrentHp = _DataAsset->CurrentHp;

	StatStruct.AttackRadius = _DataAsset->AttackRadius;
	StatStruct.AttackSweepRadius = _DataAsset->AttackSweepRadius;

	StatStruct.DetectRadius = _DataAsset->DetectRadius;
	StatStruct.PatrolRadius = _DataAsset->PatrolRadius;

	StatStruct.WalkSpeed = _DataAsset->WalkSpeed;
	StatStruct.RunSpeed = _DataAsset->RunSpeed;
	StatStruct.WalkDistanceThreshold = _DataAsset->WalkDistanceThreshold;

	StatStruct.BaseAttackPower = _DataAsset->BaseAttackPower;

	Stat->StatInit(StatStruct);
}

EMonsterState ABMonsterBase::GetCurrentState()
{
	return CurState;
}

void ABMonsterBase::DamagedEnd()
{
	AAIController* AIController = Cast<AAIController>(GetController());
	if (AIController == nullptr)
	{
		LOG_WARNING(TEXT("AIController is nullptr"));
		return;
	}

	AIController->GetBlackboardComponent()->SetValueAsBool(BBKEY_ISDAMAGED, false);

	SetCurrentState(EMonsterState::Walk);

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	if (AnimInstance == nullptr)
	{
		LOG_WARNING(TEXT("AnimInstance is Nullptr"));
		return;
	}

	AnimInstance->StopAllMontages(1.0f);
}

void ABMonsterBase::SetCurrentState(EMonsterState _InState)
{
	CurState = _InState;
}

void ABMonsterBase::SetDamagedSectionNums()
{
}

void ABMonsterBase::InitDamageTypes()
{
	//override
}

bool ABMonsterBase::AbleInteraction() const
{
	// Todo : 테스트용 코드 추후 기능완성 후 추가해주세요
	switch (CurState)
	{
	case EMonsterState::Attack:
	case EMonsterState::Flashed:
	case EMonsterState::Damaged:
	{
		return true;
	}
	case EMonsterState::Groggy:
	{
		return true;
	}

	}
	return false;
}

EInteraction ABMonsterBase::GetInteractionType() const
{
	switch (CurState)
	{
	case EMonsterState::Idle:
	{
		return EInteraction::None;
	}
	case EMonsterState::Turn:
	{
		return EInteraction::None;
	}
	case EMonsterState::Patrol:
	{
		return EInteraction::None;
	}
	case EMonsterState::Walk:
	{
		return EInteraction::None;
	}
	case EMonsterState::Run:
	{
		return EInteraction::None;
	}
	case EMonsterState::Attack:
	{
		return EInteraction::AttackMonster;
	}
	case EMonsterState::Flashed:
	{
		return EInteraction::GroggyMonster;
	}
	case EMonsterState::Damaged:
	{
		return EInteraction::GroggyMonster;
	}
	case EMonsterState::Death:
	{
		return EInteraction::None;
	}
	case EMonsterState::Groggy:
	{
		return EInteraction::GroggyMonster;
	}
	default:
		return EInteraction::None;
	}
}

FVector ABMonsterBase::GetUIPivot() const
{
	return FVector();
}