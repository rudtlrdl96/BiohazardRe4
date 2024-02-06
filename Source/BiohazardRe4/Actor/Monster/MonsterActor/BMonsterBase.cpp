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
#include "DamageType/BDMGPlayerDamage.h"
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

void ABMonsterBase::MonsterDeath()
{

}

void ABMonsterBase::Hit()
{

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
	float ResultDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

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

	//기존 애니메이션 모두 중지
	AnimInstance->StopAllMontages(0.1f);

	//체력감소
	Stat->DecreaseHp(ResultDamage);
	if (Stat->isDeath() == true)
	{
		//사망처리
		MonsterDeath();
		AIController->UnPossess();
		return ResultDamage;
	}

	if (DamageEvent.IsOfType(FPointDamageEvent::ClassID) == true)
	{
		const FPointDamageEvent* const PointDamage = (FPointDamageEvent*)&DamageEvent;
		TakePointDamage(PointDamage);
	}
	else if (DamageEvent.IsOfType(FRadialDamageEvent::ClassID) == true)
	{
		TakeRadialDamage();
	}

	Flashed();
	//추후 타입에 따라 구분해야함

	return ResultDamage;
}

void ABMonsterBase::DamagedEnd()
{
	AAIController* AIController = Cast<AAIController>(GetController());
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

void ABMonsterBase::TakePointDamage(const FPointDamageEvent* const& _DamageEvent)
{
	UBDMGPlayerDamage* CastedDamageType = Cast<UBDMGPlayerDamage>(_DamageEvent->DamageTypeClass.GetDefaultObject());

	if (CastedDamageType == nullptr)
	{
		LOG_WARNING(TEXT("DamageEvent Casting is Failed"));
		return;
	}
	EPlayerDamageType DamagedType = CastedDamageType->DamageType;

	FString DamagedPart = GetDamagedPartToString(_DamageEvent);

	//데미지 크기
	FString DamagedPower = TEXT("");
	if (DamagedType == EPlayerDamageType::Gun)
	{
		DamagedPower = TEXT("Small");
	}
	else if (DamagedType == EPlayerDamageType::Knife)
	{
		DamagedPower = TEXT("Medium");
	}
	else if (DamagedType == EPlayerDamageType::Kick)
	{
		DamagedPower = TEXT("Large");
	}
	
	FString SectionName = TEXT("");

	SectionName += DamagedPart;
	SectionName += DamagedPower;
}

void ABMonsterBase::TakeRadialDamage()
{

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
		return TEXT("Head");
	}

	bool bisLArm = SkeletalMeshComponent->BoneIsChildOf(DamagedBoneName, FName(TEXT("L_SHOULDER")));
	if (bisLArm == true || DamagedBoneName == FName(TEXT("L_SHOULDER")))
	{
		LOG_MSG(TEXT("LArm Shot"));
		return TEXT("LArm");
	}

	bool bisRArm = SkeletalMeshComponent->BoneIsChildOf(DamagedBoneName, FName(TEXT("R_SHOULDER")));
	if (bisRArm == true || DamagedBoneName == FName(TEXT("R_SHOULDER")))
	{
		LOG_MSG(TEXT("RArm Shot"));
		return TEXT("RArm");
	}

	bool bisLLeg = SkeletalMeshComponent->BoneIsChildOf(DamagedBoneName, FName(TEXT("L_THIGH")));
	if (bisLLeg == true || DamagedBoneName == FName(TEXT("L_THIGH")))
	{
		LOG_MSG(TEXT("LLeg Shot"));
		return TEXT("LLeg");
	}

	bool bisRLeg = SkeletalMeshComponent->BoneIsChildOf(DamagedBoneName, FName(TEXT("R_THIGH")));
	if (bisRLeg == true || DamagedBoneName == FName(TEXT("R_THIGH")))
	{
		LOG_MSG(TEXT("RLeg Shot"));
		return TEXT("RLeg");
	}

	return TEXT("Body");
}

void ABMonsterBase::InitDamageTypes()
{
}
