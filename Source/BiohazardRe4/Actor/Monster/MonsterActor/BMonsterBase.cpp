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

void ABMonsterBase::Flashed()
{
	if (DamagedMontage == nullptr)
	{
		LOG_WARNING(TEXT("DamagedMontage is Nullptr"));
		return;
	}

	SetCurrentState(EMonsterState::Flashed);

	int SectionNumber = FMath::RandRange(1, 3);
	FName SectionName = *FString::Printf(TEXT("FlashStart%d"), SectionNumber);

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	if (AnimInstance == nullptr)
	{
		LOG_WARNING(TEXT("AnimInstance is Nullptr"));
		return;
	}

	AnimInstance->Montage_Play(DamagedMontage, 1.0f);
	AnimInstance->Montage_JumpToSection(SectionName, DamagedMontage);
}

void ABMonsterBase::MonsterDeath(EDeathType _DeathType, const FPointDamageEvent* const& _DamageEvent)
{
	//행동트리
	AAIController* AIController = Cast<AAIController>(GetController());
	if (AIController == nullptr)
	{
		LOG_WARNING(TEXT("AIController is nullptr"));
		return;
	}

	AIController->UnPossess();

	if (_DeathType == EDeathType::Point)
	{
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

		FVector ShotDir = _DamageEvent->ShotDirection;
		ShotDir *= -1.0f;
		ShotDir.Normalize();

		FVector MonsterForward = GetActorForwardVector();
		MonsterForward.Normalize();

		double Dot = FVector::DotProduct(ShotDir, MonsterForward);

		LOG_MSG(TEXT("Monster Dead Dot is %f"), Dot);

		FName SectionName;
		if (Dot >=0.0f && Dot <= 1.0f)
		{
			SectionName = FName(TEXT("DeadFront"));
		}
		else
		{
			SectionName = FName(TEXT("DeadBack"));
		}

		//기존 애니메이션 모두 중지
		AnimInstance->StopAllMontages(0.1f);

		AnimInstance->Montage_Play(DamagedMontage, 1.0f);
		AnimInstance->Montage_JumpToSection(SectionName, DamagedMontage);
		
		SetCurrentState(EMonsterState::Death);

		DamagedMontage->bEnableAutoBlendOut = false;

		GetMesh()->SetCollisionProfileName(TEXT("NoCollision"));
	}
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


float ABMonsterBase::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float OriginResultDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance == nullptr)
	{
		LOG_WARNING(TEXT("AnimInstance is nullptr"));
		return 0.0f;
	}

	//행동트리
	AAIController* AIController = Cast<AAIController>(GetController());
	if (AIController == nullptr)
	{
		LOG_WARNING(TEXT("AIController is nullptr"));
		return 0.0f;
	}

	AIController->GetBlackboardComponent()->SetValueAsBool(BBKEY_ISDAMAGED, true);
	AIController->GetBlackboardComponent()->SetValueAsBool(BBKEY_ISNEAR, false);
	
	float ResultDamage = 0.0f;

	if (DamageEvent.IsOfType(FPointDamageEvent::ClassID) == true)
	{
		const FPointDamageEvent* const PointDamage = (FPointDamageEvent*)&DamageEvent;

		ResultDamage = TakePointDamage(PointDamage, OriginResultDamage);
	}
	else if (DamageEvent.IsOfType(FRadialDamageEvent::ClassID) == true)
	{
		TakeRadialDamage();
		Flashed();
	}

	return ResultDamage;
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

float ABMonsterBase::TakePointDamage(const FPointDamageEvent* const& _DamageEvent, float _DamagedAmount)
{
	UBDMGPlayerDamage* CastedDamageType = Cast<UBDMGPlayerDamage>(_DamageEvent->DamageTypeClass.GetDefaultObject());

	if (CastedDamageType == nullptr)
	{
		LOG_WARNING(TEXT("DamageEvent Casting is Failed"));
		return _DamagedAmount;
	}

	EPlayerDamageType DamagedType = CastedDamageType->DamageType;
	FString DamagedPart = GetDamagedPartToString(_DamageEvent);

	if (DamagedMontageSectionNums.Contains(DamagedPart) == false)
	{
		LOG_WARNING(TEXT("DamagedMontageSectionNums Index Over : %s"), *DamagedPart);
		return _DamagedAmount;
	}

	float ResultDamage = CaculateDamage(_DamagedAmount, DamagedPart, DamagedType);

	Stat->DecreaseHp(ResultDamage);
	if (Stat->isDeath() == true)
	{
		MonsterDeath(EDeathType::Point, _DamageEvent);	
		return ResultDamage;
	}

	//데미지 크기
	if (DamagedType == EPlayerDamageType::Gun)
	{
		DamagedByGun(DamagedPart);
	}
	else if (DamagedType == EPlayerDamageType::Knife)
	{
		DamagedByKnife(DamagedPart);
	}
	else if (DamagedType == EPlayerDamageType::Kick)
	{
		DamagedByKick(DamagedPart);
	}

	return ResultDamage;
}

void ABMonsterBase::TakeRadialDamage()
{
	if (Stat->isDeath() == true)
	{
		//MonsterDeath(EDeathType::Point);
		//AIController->UnPossess();
		//
		//LOG_MSG(TEXT("Monster CurSection : %s"), *(AnimInstance->Montage_GetCurrentSection().ToString()));
		//return ResultDamage;
	}
}

const FString ABMonsterBase::GetDamagedPartToString(const FPointDamageEvent* const& _DamageEvent)
{
	UPrimitiveComponent* DamagedComponent = _DamageEvent->HitInfo.GetComponent();
	FName DamagedBoneName = _DamageEvent->HitInfo.BoneName;

	USkeletalMeshComponent* SkeletalMeshComponent = nullptr;

	if (DamagedComponent && DamagedComponent->IsA(USkeletalMeshComponent::StaticClass()))
	{
		SkeletalMeshComponent = Cast<USkeletalMeshComponent>(DamagedComponent);
	}

	if (SkeletalMeshComponent == nullptr)
	{
		LOG_WARNING(TEXT("SkeletalMesh is nullptr"));
		return TEXT("");
	}

	LOG_MSG(TEXT("Damaged Bone Name is %s"), *(DamagedBoneName.ToString()));

	bool bisHead = SkeletalMeshComponent->BoneIsChildOf(DamagedBoneName, FName(TEXT("NECK_0")));
	if (bisHead == true || DamagedBoneName == FName(TEXT("NECK_0")))
	{
		LOG_MSG(TEXT("Head Shot"));
		return TEXT("HEAD");
	}

	bool bisLArm = SkeletalMeshComponent->BoneIsChildOf(DamagedBoneName, FName(TEXT("L_SHOULDER")));
	if (bisLArm == true || DamagedBoneName == FName(TEXT("L_SHOULDER")))
	{
		LOG_MSG(TEXT("LArm Shot"));
		return TEXT("LARM");
	}

	bool bisRArm = SkeletalMeshComponent->BoneIsChildOf(DamagedBoneName, FName(TEXT("R_SHOULDER")));
	if (bisRArm == true || DamagedBoneName == FName(TEXT("R_SHOULDER")))
	{
		LOG_MSG(TEXT("RArm Shot"));
		return TEXT("RARM");
	}

	bool bisLLeg = SkeletalMeshComponent->BoneIsChildOf(DamagedBoneName, FName(TEXT("L_THIGH")));
	if (bisLLeg == true || DamagedBoneName == FName(TEXT("L_THIGH")))
	{
		LOG_MSG(TEXT("LLeg Shot"));
		return TEXT("LLEG");
	}

	bool bisRLeg = SkeletalMeshComponent->BoneIsChildOf(DamagedBoneName, FName(TEXT("R_THIGH")));
	if (bisRLeg == true || DamagedBoneName == FName(TEXT("R_THIGH")))
	{
		LOG_MSG(TEXT("RLeg Shot"));
		return TEXT("RLEG");
	}

	return TEXT("BODY");
}

void ABMonsterBase::DamagedByGun(const FString& _DamagedPart)
{
	SmallDamaged(_DamagedPart);
}

void ABMonsterBase::DamagedByKnife(const FString& _DamagedPart)
{
	if (DamagedMontageSectionNums[_DamagedPart].Contains(TEXT("MEDIUM")) == false)
	{
		SmallDamaged(_DamagedPart);
	}
	else
	{
		MediumDamaged(_DamagedPart);
	}
}


void ABMonsterBase::DamagedByKick(const FString& _DamagedPart)
{
}

void ABMonsterBase::SmallDamaged(const FString& _DamagedPart)
{
	FString DamagedPower = TEXT("SMALL");

	int SectionIndex = FMath::RandRange(1, DamagedMontageSectionNums[_DamagedPart][DamagedPower]);

	FString SectionNameStr = _DamagedPart + DamagedPower + FString::FromInt(SectionIndex);
	FName SectionName(SectionNameStr);
	if (DamagedMontage->IsValidSectionName(SectionName) == false)
	{
		LOG_WARNING(TEXT("Damaged Montage Section Name is isvalid : %s"), *SectionNameStr);
	}

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	FName CurSection = AnimInstance->Montage_GetCurrentSection();
	FString CurSectionStr = CurSection.ToString();

	if (CurSectionStr.Mid(0, 4).Compare(TEXT("HEAD")) != 0)
	{
		//기존 애니메이션 모두 중지
		AnimInstance->StopAllMontages(0.1f);

		AnimInstance->Montage_Play(DamagedMontage, 1.0f);
		AnimInstance->Montage_JumpToSection(SectionName, DamagedMontage);
	}
}

void ABMonsterBase::MediumDamaged(const FString& _DamagedPart)
{
	FString DamagedPower = TEXT("MEDIUM");

	int SectionIndex = FMath::RandRange(1, DamagedMontageSectionNums[_DamagedPart][DamagedPower]);

	FString SectionNameStr = _DamagedPart + DamagedPower + FString::FromInt(SectionIndex);
	FName SectionName(SectionNameStr);

	if (DamagedMontage->IsValidSectionName(SectionName) == false)
	{
		LOG_WARNING(TEXT("Damaged Montage Section Name is isvalid : %s"), *SectionNameStr);
	}

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	//기존 애니메이션과 바뀔 애니메이션이 같으면 안바꿈
	if (AnimInstance->Montage_GetCurrentSection().Compare(SectionName) != 0)
	{
		AnimInstance->StopAllMontages(0.1f);

		AnimInstance->Montage_Play(DamagedMontage, 1.0f);
		AnimInstance->Montage_JumpToSection(SectionName, DamagedMontage);
	}
}

void ABMonsterBase::LargeDamaged(const FString& _DamagedPart)
{
}

float ABMonsterBase::CaculateDamage(float _OriginDamage, const FString& _DamagedPart, EPlayerDamageType _PlayerDamageType)
{
	//체력감소 - 기본 딜
	if (_DamagedPart.Compare(TEXT("HEAD")) == 0)
	{
		if (_PlayerDamageType == EPlayerDamageType::Gun)
		{
			_OriginDamage *= 1.2f;
		}
		else if (_PlayerDamageType != EPlayerDamageType::Gun)
		{
			_OriginDamage *= 1.5f;
		}
	}
	else if (_DamagedPart.Compare(TEXT("BODY")) != 0 && _DamagedPart.Compare(TEXT("HEAD")) != 0)
	{
		_OriginDamage *= 0.5f;
	}
	else
	{
		_OriginDamage *= 1.0f;
	}

	//치명타
	int Critical = FMath::RandRange(1, 100);
	bool isCritical = false;

	if (Critical >= 1 && Critical <= 6)
	{
		isCritical = true;
	}

	if (isCritical == true)
	{
		if (_PlayerDamageType == EPlayerDamageType::Gun)
		{
			_OriginDamage *= 3.0f;
		}
		else if (_PlayerDamageType == EPlayerDamageType::Knife)
		{
			_OriginDamage *= 1.335f;
		}
		else if (_PlayerDamageType == EPlayerDamageType::Kick)
		{
			_OriginDamage *= 4.0f;
		}
	}

	FString CriticalStr = "";
	CriticalStr = (isCritical) ? TEXT("[Critical]") : TEXT(" ");
	LOG_MSG(TEXT("Monster is Damaged : %f %s, RemainHp = %f"), _OriginDamage, *CriticalStr, Stat->GetCurrentHp());

	return _OriginDamage;
}

void ABMonsterBase::InitDamageTypes()
{
}


bool ABMonsterBase::AbleInteraction() const
{
	return bisAbleInteraction;
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
		break;
	}
}

FVector ABMonsterBase::GetUIPivot() const
{
	return FVector();
}