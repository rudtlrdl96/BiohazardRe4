// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Monster/MonsterActor/BMonsterBase.h"
#include "AIController.h"
#include "Kismet/GameplayStatics.h"
#include "Components/CapsuleComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

#include "BiohazardRe4.h"
#include "Actor/Monster/Define/MonsterDefine.h"
#include "Actor/Monster/DataAsset/BMonsterStatData.h"
#include "Actor/Monster/Component/BMonsterStatComponent.h"

// Sets default values
ABMonsterBase::ABMonsterBase()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.TickInterval = 0.05f;

	bUseControllerRotationYaw = false;

	Stat = CreateDefaultSubobject<UBMonsterStatComponent>(TEXT("Stat"));
}

void ABMonsterBase::BeginPlay()
{
	Super::BeginPlay();

	SetDamagedSectionNums();
}

void ABMonsterBase::Tick(float _DeltaTime)
{
	Super::Tick(_DeltaTime);
	BurstJumpUpdate();
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

	AnimInstance->Montage_Play(AttackMontage);
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

void ABMonsterBase::SetDamagedSectionNums()
{
}

void ABMonsterBase::InitDamageTypes()
{
	//override
}