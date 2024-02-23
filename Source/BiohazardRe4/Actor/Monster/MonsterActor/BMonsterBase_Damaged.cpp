
#include "Actor/Monster/MonsterActor/BMonsterBase.h"

#include "AIController.h"
#include "Engine/DamageEvents.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"

#include "BiohazardRe4.h"
#include "Actor/Monster/Define/MonsterDefine.h"
#include "Actor/Monster/Component/BMonsterStatComponent.h"
#include "Actor/Monster/Interface/BMonsterAnimInterface.h"
#include "Actor/Monster/AIController/BAIBasicMonsterController.h"

float ABMonsterBase::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float OriginResultDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	//0 -> Normal, 1->Point, 2-> Radial
	int TypeID = DamageEvent.GetTypeID();
	
	float ResultDamage = 0.0f;
	
	if (DamageEvent.IsOfType(FPointDamageEvent::ClassID) == true)
	{
		ResultDamage = TakePointDamage(DamageEvent, DamageCauser, OriginResultDamage);
	}
	else if (DamageEvent.IsOfType(FRadialDamageEvent::ClassID) == true)
	{
		ResultDamage = TakeRadialDamage(DamageEvent, DamageCauser, OriginResultDamage);
	}
	else if (DamageEvent.IsOfType(FDamageEvent::ClassID) == true)
	{
		ResultDamage = TakeNormalDamage(DamageEvent, DamageCauser, OriginResultDamage);
	}

	return ResultDamage;
}

float ABMonsterBase::TakePointDamage(const FDamageEvent& _DamageEvent, const AActor* DamageCauser, float _DamagedAmount)
{
	const FPointDamageEvent* const PointDamage = (FPointDamageEvent*)&_DamageEvent;
	if (PointDamage == nullptr)
	{
		LOG_WARNING(TEXT("DamageEvent Casting is Failed"));
		return _DamagedAmount;
	}

	UBDMGPlayerDamage* CastedDamageType = Cast<UBDMGPlayerDamage>(PointDamage->DamageTypeClass.GetDefaultObject());

	if (CastedDamageType == nullptr)
	{
		LOG_WARNING(TEXT("DamageType Casting is Failed"));
		return _DamagedAmount;
	}

	EPlayerDamageType DamagedType = CastedDamageType->DamageType;
	FString DamagedPart = GetDamagedPartToString(PointDamage);

	CreateBloodParticle(PointDamage->HitInfo.Location, PointDamage->ShotDirection.Rotation());

	if (DamagedMontageSectionNums.Contains(DamagedPart) == false)
	{
		LOG_WARNING(TEXT("DamagedMontageSectionNums Index Over : %s"), *DamagedPart);
		return _DamagedAmount;
	}

	float ResultDamage = CaculatePointDamage(_DamagedAmount, DamagedPart, DamagedType);

	Stat->DecreaseHp(ResultDamage);
	if (Stat->isDeath() == true)
	{
		MonsterDeath(EDeathType::Point, _DamageEvent, DamageCauser);
		return ResultDamage;
	}

	if (GetCurrentState() == EMonsterState::Attack || GetCurrentState() == EMonsterState::Burst)
	{
		return ResultDamage;
	}

	if (GetCurrentState() == EMonsterState::Groggy && DamagedPart.Compare(TEXT("HEAD")) != 0)
	{
		return ResultDamage;
	}

	if (DamagedPart.Compare(TEXT("HEAD")) != 0)
	{
		GroggyAmount += 0.5f;

		if (bIsDamaged == false && bIsDamagedCooltime == false)
		{
			bIsDamagedCooltime = true;
			bIsDamaged = true;

			PlaySound(ESoundType::Damaged);
			GetWorldTimerManager().SetTimer(TimerHandle, [this] {bIsDamagedCooltime = false; }, 1.5f, false);
		}

		if (GroggyAmount <= GetGroggyThreshold())
		{
			return ResultDamage;
		}
	}

	SetCurrentState(EMonsterState::Groggy);
	
	if (DamagedPart.Compare(TEXT("HEAD")) != 0)
	{
		PlaySound(ESoundType::GeneralGroggy);
	}
	else
	{
		PlaySound(ESoundType::HeadShotGroggy);
	}

	//데미지 크기
	if (DamagedType == EPlayerDamageType::Gun)
	{
		DamagedByGun(DamagedPart);
	}
	else
	{
		EPlayerDamageType PlayerDamageType = DamagedType;

		FString EnumToString = TEXT("Invalid");
		const UEnum* CharStateEnum = FindObject<UEnum>(ANY_PACKAGE, TEXT("EPlayerDamageType"), true);

		if (CharStateEnum)
		{
			EnumToString = CharStateEnum->GetNameStringByValue((int64)PlayerDamageType);
			LOG_MSG(TEXT("ApplyPointDamage is Only Able by Gun. This DamageType is %s"), *EnumToString);
		}
	}

	return ResultDamage;
}

float ABMonsterBase::TakeRadialDamage(const FDamageEvent& _DamageEvent, const AActor* DamageCauser, float _DamagedAmount)
{
	const FRadialDamageEvent* const RadialDamage = (FRadialDamageEvent*)&_DamageEvent;
	if (RadialDamage == nullptr)
	{
		LOG_WARNING(TEXT("DamageEvent Casting is Failed"));
		return _DamagedAmount;
	}

	AAIController* AIController = Cast<AAIController>(GetController());
	if (AIController == nullptr)
	{
		LOG_WARNING(TEXT("AIController is nullptr"));
		return 0.0f;
	}

	AIController->GetBlackboardComponent()->SetValueAsBool(BBKEY_ISNEAR, false);
	AIController->GetBlackboardComponent()->SetValueAsBool(BBKEY_ISDAMAGED, true);

	UBDMGPlayerDamage* CastedDamageType = Cast<UBDMGPlayerDamage>(RadialDamage->DamageTypeClass.GetDefaultObject());
	
	if (CastedDamageType == nullptr)
	{
		LOG_WARNING(TEXT("DamageType Casting is Failed"));
		return _DamagedAmount;
	}

	Stat->DecreaseHp(_DamagedAmount);
	if (Stat->isDeath() == true)
	{
		MonsterDeath(EDeathType::Grenade, _DamageEvent, DamageCauser);
		AIController->UnPossess();
		return _DamagedAmount;
	}

	EPlayerDamageType DamagedType = CastedDamageType->DamageType;

	if (DamagedType == EPlayerDamageType::Grenade)
	{
		DamagedByGrenade(_DamageEvent, _DamagedAmount);
	}
	else if(DamagedType == EPlayerDamageType::FlashBang)
	{
		DamagedByFlashed();
	}

	return 0.0f;
}

//ApplyDamage (Not Point, Radial)
float ABMonsterBase::TakeNormalDamage(const FDamageEvent& _DamageEvent, const AActor* DamageCauser, float _DamagedAmount)
{
	UBDMGPlayerDamage* CastedDamageType = Cast<UBDMGPlayerDamage>(_DamageEvent.DamageTypeClass.GetDefaultObject());

	if (CastedDamageType == nullptr)
	{
		LOG_WARNING(TEXT("DamageEvent Casting is Failed"));
		return _DamagedAmount;
	}

	if (GetCurrentState() == EMonsterState::Death)
	{
		return 0.0f;
	}

	EPlayerDamageType DamagedType = CastedDamageType->DamageType;
	float ResultDamage = 0.0f;

	if (DamagedType == EPlayerDamageType::Kick)
	{
		if (GetCurrentState() != EMonsterState::Burst)
		{
			ResultDamage = CaculateNormalDamage(_DamagedAmount, DamagedType);
			Stat->DecreaseHp(ResultDamage);

			LOG_MSG(TEXT("Monster Damaged By Kick"));

			if (Stat->isDeath() == true)
			{
				MonsterDeath(EDeathType::Kick, _DamageEvent, DamageCauser);
				return ResultDamage;
			}

			DamagedByKick(_DamageEvent, DamageCauser);
		}
	}
	else if (DamagedType == EPlayerDamageType::Knife)
	{
		if(bIsKnifeDamaged == false)
		{
			bIsKnifeDamaged = true;
			GetWorldTimerManager().SetTimer(KnifeDamagedTimerHandle, [this] {bIsKnifeDamaged = false; }, 0.4f, false);

			LOG_MSG(TEXT("Monster Damaged By Knife"));

			ResultDamage = CaculateNormalDamage(_DamagedAmount, DamagedType);
			Stat->DecreaseHp(ResultDamage);

			if (Stat->isDeath() == true)
			{
				MonsterDeath(EDeathType::Knife, _DamageEvent, DamageCauser);
				return ResultDamage;
			}

			DamagedByKnife(_DamageEvent);
		}
	}
	else
	{
		EPlayerDamageType PlayerDamageType = DamagedType;

		FString EnumToString = TEXT("Invalid");
		const UEnum* CharStateEnum = FindObject<UEnum>(ANY_PACKAGE, TEXT("EPlayerDamageType"), true);

		if (CharStateEnum)
		{
			EnumToString = CharStateEnum->GetNameStringByValue((int64)PlayerDamageType);
			LOG_MSG(TEXT("ApplyPointDamage is Only Able by Kick. This DamageType is %s"), *EnumToString);
		}
	}

	return ResultDamage;
}

void ABMonsterBase::MonsterDeath(EDeathType _DeathType, const FDamageEvent& _DamageEvent, const AActor* DamageCauser)
{
	switch (_DeathType)
	{
	case EDeathType::Point:
		MonsterDeathByPoint(_DamageEvent);
		break;
	case EDeathType::Kick:
		MonsterDeathByKick(_DamageEvent, DamageCauser);
		break;
	case EDeathType::Knife:
		MonsterDeathByKnife(_DamageEvent, DamageCauser);
		break;
	case EDeathType::Grenade:
		MonsterDeathByGrenade(_DamageEvent, DamageCauser);
		break;
	default:
		break;
	}
}

void ABMonsterBase::MonsterDeathByPoint(const FDamageEvent& _DamageEvent)
{
	//행동트리
	AAIController* AIController = Cast<AAIController>(GetController());
	if (AIController == nullptr)
	{
		LOG_WARNING(TEXT("AIController is nullptr"));
		return;
	}

	AIController->UnPossess();
	PlaySound(ESoundType::PointDeath);

	const FPointDamageEvent* const PointDamage = (FPointDamageEvent*)&_DamageEvent;
	if (PointDamage == nullptr)
	{
		LOG_WARNING(TEXT("DamageEvent Casting is Failed"));
		return;
	}

	FVector ShotDir = PointDamage->ShotDirection;
	ShotDir *= -1.0f;
	ShotDir.Normalize();

	FVector MonsterForward = GetActorForwardVector();
	MonsterForward.Normalize();

	double Dot = FVector::DotProduct(ShotDir, MonsterForward);

	LOG_MSG(TEXT("Monster Dead Dot is %f"), Dot);

	FName SectionName;
	if (Dot >= 0.0f && Dot <= 1.0f)
	{
		SectionName = FName(TEXT("DeadFront"));
	}
	else
	{
		SectionName = FName(TEXT("DeadBack"));
	}

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	//기존 애니메이션 모두 중지
	AnimInstance->StopAllMontages(0.1f);

	AnimInstance->Montage_Play(DamagedMontage, 1.0f);
	AnimInstance->Montage_JumpToSection(SectionName, DamagedMontage);

	SetCurrentState(EMonsterState::Death);
	AllCollisionOff();

	DamagedMontage->bEnableAutoBlendOut = false;
}

void ABMonsterBase::MonsterDeathByKick(const FDamageEvent& _DamageEvent, const AActor* DamageCauser)
{
	//행동트리
	AAIController* AIController = Cast<AAIController>(GetController());
	if (AIController == nullptr)
	{
		LOG_WARNING(TEXT("AIController is nullptr"));
		return;
	}

	AIController->UnPossess();
	SetCurrentState(EMonsterState::Burst);

	FVector CauserLocation = DamageCauser->GetActorLocation();
	FVector MyLocation = GetActorLocation();
	MyLocation.Z += 176.0f;

	FVector LaunchDir = MyLocation - CauserLocation;
	LaunchDir.Normalize();
	//피격 각도
	FString SectionStr = GetBurstJumpSectionName(MyLocation, CauserLocation);

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	//기존 애니메이션 모두 중지
	AnimInstance->StopAllMontages(0.1f);
	AnimInstance->Montage_Play(DamagedMontage, 1.0f);
	AnimInstance->Montage_JumpToSection(FName(SectionStr + TEXT("Start")), DamagedMontage);
	AnimInstance->SetRootMotionMode(ERootMotionMode::IgnoreRootMotion);

	TWeakObjectPtr<UAnimInstance> WeakAnimInstance = AnimInstance;

	TFunction<void()> Lambda = [this, WeakAnimInstance, SectionStr]()
		{
			SetCurrentState(EMonsterState::Death);
			WeakAnimInstance.Get()->Montage_JumpToSection(FName(SectionStr + TEXT("End")), DamagedMontage);
			WeakAnimInstance->SetRootMotionMode(ERootMotionMode::RootMotionFromEverything);

			GetWorldTimerManager().SetTimer(CollisionOffTimer, [this] {AllCollisionOff(); }, 1.0f, false);
		};

	BurstJumpStart(LaunchDir, 600.0f, Lambda, false, false);
}

void ABMonsterBase::MonsterDeathByKnife(const FDamageEvent& _DamageEvent, const AActor* DamageCauser)
{
	//행동트리
	AAIController* AIController = Cast<AAIController>(GetController());
	if (AIController == nullptr)
	{
		LOG_WARNING(TEXT("AIController is nullptr"));
		return;
	}

	PlaySound(ESoundType::PointDeath);
	AIController->UnPossess();

	FVector ShotDir = DamageCauser->GetActorLocation() - GetActorLocation();
	ShotDir.Normalize();

	FVector MonsterForward = GetActorForwardVector();
	MonsterForward.Normalize();

	double Dot = FVector::DotProduct(ShotDir, MonsterForward);

	LOG_MSG(TEXT("Monster Dead Dot is %f"), Dot);

	FName SectionName;
	if (Dot >= 0.0f && Dot <= 1.0f)
	{
		SectionName = FName(TEXT("DeadFront"));
	}
	else
	{
		SectionName = FName(TEXT("DeadBack"));
	}

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	//기존 애니메이션 모두 중지
	AnimInstance->StopAllMontages(0.1f);

	AnimInstance->Montage_Play(DamagedMontage, 1.0f);
	AnimInstance->Montage_JumpToSection(SectionName, DamagedMontage);

	SetCurrentState(EMonsterState::Death);
	AllCollisionOff();

	DamagedMontage->bEnableAutoBlendOut = false;
}

void ABMonsterBase::MonsterDeathByGrenade(const FDamageEvent& _DamageEvent, const AActor* DamageCauser)
{
	const FRadialDamageEvent* const RadialDamage = (FRadialDamageEvent*)&_DamageEvent;
	if (RadialDamage == nullptr)
	{
		LOG_WARNING(TEXT("DamageEvent Casting is Failed"));
		return;
	}

	FVector MyPos = GetActorLocation();
	FVector RadialPos = RadialDamage->Origin;
	MyPos.Z += 88.0f;

	FVector LaunchDir = MyPos - RadialPos;
	LaunchDir.Normalize();

	float LaunchPower = 900.0f;

	FString SectionStr = GetBurstJumpSectionName(MyPos, RadialPos);

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	SetCurrentState(EMonsterState::Burst);

	//기존 애니메이션 모두 중지
	AnimInstance->StopAllMontages(0.1f);
	AnimInstance->Montage_Play(DamagedMontage, 1.0f);
	AnimInstance->Montage_JumpToSection(FName(SectionStr + TEXT("Start")), DamagedMontage);
	AnimInstance->SetRootMotionMode(ERootMotionMode::IgnoreRootMotion);

	TWeakObjectPtr<UAnimInstance> WeakAnimInstance = AnimInstance;

	TFunction<void()> Lambda = [this, WeakAnimInstance, SectionStr]()
		{
			SetCurrentState(EMonsterState::Death);
			WeakAnimInstance.Get()->Montage_JumpToSection(FName(SectionStr + TEXT("End")), DamagedMontage);
			WeakAnimInstance->SetRootMotionMode(ERootMotionMode::RootMotionFromEverything);

			GetWorldTimerManager().SetTimer(CollisionOffTimer, [this] {AllCollisionOff(); }, 1.0f, false);
		};

	BurstJumpStart(LaunchDir, LaunchPower, Lambda, false, false);
}

void ABMonsterBase::DamagedByGun(const FString& _DamagedPart)
{
	SmallDamaged(_DamagedPart);
}

void ABMonsterBase::DamagedByKnife(const FDamageEvent& _DamageEvent)
{
	if (GetCurrentState() == EMonsterState::Attack)
	{
		return;
	}

	if (GetCurrentState() == EMonsterState::Groggy)
	{
		return;
	}

	GroggyAmount += 0.25f;

	if (bIsDamaged == false && bIsDamagedCooltime == false)
	{
		bIsDamagedCooltime = true;
		bIsDamaged = true;

		PlaySound(ESoundType::Damaged);
		GetWorldTimerManager().SetTimer(TimerHandle, [this] {bIsDamagedCooltime = false; }, 1.0f, false);
	} 

	LOG_MSG(TEXT("Groggy %f"), GroggyAmount);

	if (GroggyAmount <= GetGroggyThreshold())
	{
		return;
	}

	PlaySound(ESoundType::GeneralGroggy);

	GroggyAmount = 0.0f;

	ABAIBasicMonsterController* AIController = Cast<ABAIBasicMonsterController>(GetController());
	if (AIController == nullptr)
	{
		LOG_WARNING(TEXT("AIController is nullptr"));
		return;
	}

	AIController->GetBlackboardComponent()->SetValueAsBool(BBKEY_ISNEAR, false);
	AIController->GetBlackboardComponent()->SetValueAsBool(BBKEY_ISDAMAGED, true);
	//AIController->Announce();

	SetCurrentState(EMonsterState::Groggy);
	MediumDamaged(FString(TEXT("Body")));
}

void ABMonsterBase::DamagedByKick(const FDamageEvent& _DamageEvent, const AActor* DamageCauser)
{
	PlaySound(ESoundType::Kicked);
	SetCurrentState(EMonsterState::Burst);

	AAIController* AIController = Cast<AAIController>(GetController());
	if (AIController == nullptr)
	{
		LOG_WARNING(TEXT("AIController is nullptr"));
		return;
	}

	AIController->GetBlackboardComponent()->SetValueAsBool(BBKEY_ISNEAR, false);
	AIController->GetBlackboardComponent()->SetValueAsBool(BBKEY_ISDAMAGED, true);

	FVector CauserLocation = DamageCauser->GetActorLocation();
	FVector MyLocation = GetActorLocation();
	MyLocation.Z += 176.0f;

	FVector LaunchDir = MyLocation - CauserLocation;
	LaunchDir.Normalize();

	FString SectionStr = GetBurstJumpSectionName(MyLocation, CauserLocation);

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	
	//기존 애니메이션 모두 중지
	AnimInstance->StopAllMontages(0.1f);
	AnimInstance->Montage_Play(DamagedMontage, 1.0f);
	AnimInstance->Montage_JumpToSection(FName(SectionStr + TEXT("Start")), DamagedMontage);
	AnimInstance->SetRootMotionMode(ERootMotionMode::IgnoreRootMotion);

	TWeakObjectPtr<UAnimInstance> WeakAnimInstance = AnimInstance;
	
	TFunction<void()> Lambda = [this, WeakAnimInstance, SectionStr]()
		{
			WeakAnimInstance.Get()->Montage_JumpToSection(FName(SectionStr + TEXT("End")), DamagedMontage);
			WeakAnimInstance->SetRootMotionMode(ERootMotionMode::RootMotionFromEverything);
		};
	
	BurstJumpStart(LaunchDir, 600.0f, Lambda, true, true);
}

void ABMonsterBase::DamagedByGrenade(const FDamageEvent& _DamageEvent, float _DamagedAmount)
{
	const FRadialDamageEvent* const RadialDamage = (FRadialDamageEvent*)&_DamageEvent;
	if (RadialDamage == nullptr)
	{
		LOG_WARNING(TEXT("DamageEvent Casting is Failed"));
		return;
	}

	FVector MyPos = GetActorLocation();
	MyPos.Z += 88.0f;

	FVector RadialPos = RadialDamage->Origin;
	float DamageRatio = _DamagedAmount / RadialDamage->Params.BaseDamage;

	FVector LaunchDir = MyPos - RadialPos;
	LaunchDir.Normalize();

	float LaunchPower = 1200.0f;
	LaunchPower = FMath::Clamp(LaunchPower * DamageRatio, 700.0f, 1200.0f);

	LOG_MSG(TEXT("LaunchPower : %f"), LaunchPower);

	FString SectionStr = GetBurstJumpSectionName(MyPos, RadialPos);

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	SetCurrentState(EMonsterState::Burst);

	//기존 애니메이션 모두 중지
	AnimInstance->StopAllMontages(0.1f);
	AnimInstance->Montage_Play(DamagedMontage, 1.0f);
	AnimInstance->Montage_JumpToSection(FName(SectionStr + TEXT("Start")), DamagedMontage);
	AnimInstance->SetRootMotionMode(ERootMotionMode::IgnoreRootMotion);

	TWeakObjectPtr<UAnimInstance> WeakAnimInstance = AnimInstance;

	TFunction<void()> Lambda = [this, WeakAnimInstance, SectionStr]()
		{
			WeakAnimInstance.Get()->Montage_JumpToSection(FName(SectionStr + TEXT("End")), DamagedMontage);
			WeakAnimInstance->SetRootMotionMode(ERootMotionMode::RootMotionFromEverything);
		};

	BurstJumpStart(LaunchDir, LaunchPower, Lambda, false, false);
}

void ABMonsterBase::BurstJumpStart(FVector _Direction, float _JumpPower, TFunction<void()> _Function, bool _XYOverride, bool _ZOverride)
{
	LaunchCharacter(_Direction * _JumpPower, _XYOverride, _ZOverride);
	OnLandedByBurstJump.BindLambda(_Function);
}

void ABMonsterBase::SmallDamaged(const FString& _DamagedPart)
{
	GroggyAmount = 0.0f;

	//행동트리
	AAIController* AIController = Cast<AAIController>(GetController());
	if (AIController == nullptr)
	{
		LOG_WARNING(TEXT("AIController is nullptr"));
		return;
	}

	AIController->GetBlackboardComponent()->SetValueAsBool(BBKEY_ISNEAR, false);
	AIController->GetBlackboardComponent()->SetValueAsBool(BBKEY_ISDAMAGED, true);

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

	if (CurSectionStr.Compare(SectionNameStr) != 0)
	{
		//기존 애니메이션 모두 중지
		AnimInstance->StopAllMontages(0.1f);

		AnimInstance->Montage_Play(DamagedMontage, 1.0f);
		AnimInstance->Montage_JumpToSection(SectionName, DamagedMontage);
	}
}

const FString ABMonsterBase::GetBurstJumpSectionName(FVector _MyLocation, FVector _CauserLocation)
{
	//피격 각도
	FVector MonsterToCauserVector = _CauserLocation - _MyLocation;
	MonsterToCauserVector.Z = 0.0f;
	MonsterToCauserVector.Normalize();

	FVector MyForwardVector = GetActorForwardVector();
	MyForwardVector.Z = 0.0f;
	MyForwardVector.Normalize();

	double DotProductValue = FVector::DotProduct(MonsterToCauserVector, MyForwardVector);
	double Radian = FMath::Acos(DotProductValue);
	double Degree = FMath::RadiansToDegrees(Radian);

	FString ReturnSectionStr = TEXT("Burst");

	if (Degree >= 0.0f && Degree < 45.0f)
	{
		ReturnSectionStr += TEXT("Front");
	}
	else if (Degree > 135.0f && Degree <= 180.0f)
	{
		ReturnSectionStr += TEXT("Back");
	}
	else
	{
		FVector MyRightVector = GetActorRightVector();
		double SideDotProductValue = FVector::DotProduct(MonsterToCauserVector, MyRightVector);
		
		if (SideDotProductValue < 0.0f)
		{
			ReturnSectionStr += TEXT("Left");
		}
		else
		{
			ReturnSectionStr += TEXT("Right");
		}
	}

	return ReturnSectionStr;
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


float ABMonsterBase::CaculatePointDamage(float _OriginDamage, const FString& _DamagedPart, const EPlayerDamageType _PlayerDamageType)
{
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
		_OriginDamage = CaculateCriticalDamage(_OriginDamage, _PlayerDamageType);
	}

	FString CriticalStr = "";
	CriticalStr = (isCritical) ? TEXT("[Critical]") : TEXT(" ");
	LOG_MSG(TEXT("Monster is Damaged : %f %s, RemainHp = %f"), _OriginDamage, *CriticalStr, Stat->GetCurrentHp() - _OriginDamage);

	return _OriginDamage;
}

float ABMonsterBase::CaculateNormalDamage(float _OriginDamage, const EPlayerDamageType _PlayerDamageType)
{
	int Critical = FMath::RandRange(1, 100);
	bool isCritical = false;

	if (Critical >= 1 && Critical <= 6)
	{
		isCritical = true;
	}

	if (isCritical == true)
	{
		_OriginDamage = CaculateCriticalDamage(_OriginDamage, _PlayerDamageType);
	}

	return _OriginDamage;
}

float ABMonsterBase::CaculateCriticalDamage(float _OriginDamage, const EPlayerDamageType _PlayerDamageType)
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

	return _OriginDamage;
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

void ABMonsterBase::Parry()
{
	if (isAbleParring() == false)
	{
		LOG_MSG(TEXT("OnParry in Not ParryTime"));
		return;
	}

	AAIController* AIController = Cast<AAIController>(GetController());
	if (AIController == nullptr)
	{
		LOG_WARNING(TEXT("AIController is nullptr"));
		return;
	}

	AIController->GetBlackboardComponent()->SetValueAsBool(BBKEY_ISNEAR, false);
	AIController->GetBlackboardComponent()->SetValueAsBool(BBKEY_ISDAMAGED, true);

	ParryTimeOff();
	AttackOff();

	PlaySound(ESoundType::Parry);
	SetCurrentState(EMonsterState::Groggy);

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	if (AnimInstance == nullptr)
	{
		LOG_WARNING(TEXT("AnimInstance is Nullptr"));
		return;
	}

	AnimInstance->Montage_Play(ParriedMontage, 1.0f);
}

void ABMonsterBase::DamagedByFlashed()
{
	if (DamagedMontage == nullptr)
	{
		LOG_WARNING(TEXT("DamagedMontage is Nullptr"));
		return;
	}

	SetCurrentState(EMonsterState::Groggy);

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

void ABMonsterBase::BurstJumpUpdate()
{
	if (GetCurrentState() == EMonsterState::Burst)
	{
		if (GetCharacterMovement()->IsMovingOnGround() == true)
		{
			OnLandedByBurstJump.ExecuteIfBound();
			OnLandedByBurstJump.Unbind();
		}
	}
}



void ABMonsterBase::CreateBloodParticle(FVector _Location, FRotator _Rotator)
{
	_Rotator.Normalize();
	FTransform ParticleTransform;

	ParticleTransform.SetScale3D(FVector(0.5f, 0.5f, 0.5f));
	ParticleTransform.SetLocation(_Location);
	ParticleTransform.SetRotation(_Rotator.Quaternion());

	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), BloodParticle, ParticleTransform, true);
}