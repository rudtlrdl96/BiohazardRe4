
#include "Actor/Monster/MonsterActor/BMonsterBase.h"

#include "AIController.h"
#include "Engine/DamageEvents.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "BiohazardRe4.h"
#include "Actor/Monster/Define/MonsterDefine.h"
#include "Actor/Monster/Component/BMonsterStatComponent.h"

float ABMonsterBase::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float OriginResultDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	SetCurrentState(EMonsterState::Damaged);

	//0 -> Normal, 1->Point, 2-> Radial
	int TypeID = DamageEvent.GetTypeID();

	LOG_MSG(TEXT("Damaged TypeID is %d"), TypeID);

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
		ResultDamage = TakePointDamage(DamageEvent, OriginResultDamage);
	}
	else if (DamageEvent.IsOfType(FRadialDamageEvent::ClassID) == true)
	{
		TakeRadialDamage();
		Flashed();
	}
	else if (DamageEvent.IsOfType(FDamageEvent::ClassID) == true)
	{
		ResultDamage = TakeNormalDamage(DamageEvent, DamageCauser, OriginResultDamage);
	}

	return ResultDamage;
}

float ABMonsterBase::TakePointDamage(const FDamageEvent& _DamageEvent, float _DamagedAmount)
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

	if (DamagedMontageSectionNums.Contains(DamagedPart) == false)
	{
		LOG_WARNING(TEXT("DamagedMontageSectionNums Index Over : %s"), *DamagedPart);
		return _DamagedAmount;
	}

	float ResultDamage = CaculatePointDamage(_DamagedAmount, DamagedPart, DamagedType);

	Stat->DecreaseHp(ResultDamage);
	if (Stat->isDeath() == true)
	{
		MonsterDeathByPoint(_DamageEvent);
		return ResultDamage;
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

//ApplyDamage (Not Point, Radial)
float ABMonsterBase::TakeNormalDamage(const FDamageEvent& _DamageEvent, const AActor* DamageCauser, float _DamagedAmount)
{
	UBDMGPlayerDamage* CastedDamageType = Cast<UBDMGPlayerDamage>(_DamageEvent.DamageTypeClass.GetDefaultObject());

	if (CastedDamageType == nullptr)
	{
		LOG_WARNING(TEXT("DamageEvent Casting is Failed"));
		return _DamagedAmount;
	}

	EPlayerDamageType DamagedType = CastedDamageType->DamageType;

	float ResultDamage = CaculateNormalDamage(_DamagedAmount, DamagedType);
	Stat->DecreaseHp(ResultDamage);

	if (Stat->isDeath() == true)
	{
		MonsterDeathByKick(_DamageEvent, DamageCauser);
		return ResultDamage;
	}

		//데미지 크기
	if (DamagedType == EPlayerDamageType::Kick)
	{
		DamagedByKick(_DamageEvent, DamageCauser);
	}
	else if (DamagedType == EPlayerDamageType::Knife)
	{
		DamagedByKnife(_DamageEvent);
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
	GetMesh()->SetCollisionProfileName(TEXT("NoCollision"));

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

	SetCurrentState(EMonsterState::Kicked);

	FVector CauserLocation = DamageCauser->GetActorLocation();
	FVector MyLocation = GetActorLocation();

	FString KickSectionStr = TEXT("Kick");

	//피격 각도
	FVector CauserToMonsterVector = MyLocation - CauserLocation;
	CauserToMonsterVector.Z = 0;
	CauserToMonsterVector.Normalize();

	FVector MyForwardVector = GetActorForwardVector();
	MyForwardVector.Z = 0;
	MyForwardVector.Normalize();

	double DotProductValue = FVector::DotProduct(-CauserToMonsterVector, MyForwardVector);
	double Radian = FMath::Acos(DotProductValue);
	double Degree = FMath::RadiansToDegrees(Radian);

	LOG_MSG(TEXT("Kick Angle is %f"), Degree);

	if (Degree >= 0.0f && Degree < 45.0f)
	{
		KickSectionStr += TEXT("Front");
	}
	else if (Degree > 135.0f && Degree <= 180.0f)
	{
		KickSectionStr += TEXT("Back");
	}
	else
	{
		KickSectionStr += TEXT("Side");
	}

	//날아가는 각도
	FVector LaunchDirXY = CauserToMonsterVector;
	float LaunchZ = 1.0f;
	LaunchDirXY.Z = LaunchZ;

	FVector LaunchDir = FVector(LaunchDirXY.X, LaunchDirXY.Y, LaunchZ);
	LaunchDir.Normalize();

	LaunchCharacter(LaunchDir * 500.0f, false, false);

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	//기존 애니메이션 모두 중지
	AnimInstance->StopAllMontages(0.1f);
	AnimInstance->Montage_Play(DamagedMontage, 1.0f);
	AnimInstance->Montage_JumpToSection(FName(KickSectionStr + TEXT("Start")), DamagedMontage);
	AnimInstance->SetRootMotionMode(ERootMotionMode::NoRootMotionExtraction);

	TWeakObjectPtr<UAnimInstance> WeakAnimInstance = AnimInstance;

	OnLandedByKickJump.BindLambda([this, WeakAnimInstance, KickSectionStr]()
		{
			SetCurrentState(EMonsterState::Death);

			WeakAnimInstance.Get()->StopAllMontages(0.1f);
			WeakAnimInstance.Get()->Montage_Play(DamagedMontage, 1.0f);
			WeakAnimInstance.Get()->Montage_JumpToSection(FName(KickSectionStr + TEXT("End")), DamagedMontage);
			WeakAnimInstance.Get()->SetRootMotionMode(ERootMotionMode::RootMotionFromEverything);
		}
	);
}

void ABMonsterBase::DamagedByGun(const FString& _DamagedPart)
{
	SmallDamaged(_DamagedPart);
}

void ABMonsterBase::DamagedByKnife(const FDamageEvent& _DamageEvent)
{
	//if (DamagedMontageSectionNums[_DamagedPart].Contains(TEXT("MEDIUM")) == false)
	//{
	//	SmallDamaged(_DamagedPart);
	//}
	//else
	//{
	//	MediumDamaged(_DamagedPart);
	//}
}

void ABMonsterBase::DamagedByKick(const FDamageEvent& _DamageEvent, const AActor* DamageCauser)
{
	SetCurrentState(EMonsterState::Kicked);

	FVector CauserLocation = DamageCauser->GetActorLocation();
	FVector MyLocation = GetActorLocation();

	FString KickSectionStr = TEXT("Kick");

	//피격 각도
	FVector CauserToMonsterVector = MyLocation - CauserLocation;
	CauserToMonsterVector.Z = 0;
	CauserToMonsterVector.Normalize();

	FVector MyForwardVector = GetActorForwardVector();
	MyForwardVector.Z = 0;
	MyForwardVector.Normalize();

	double DotProductValue = FVector::DotProduct(-CauserToMonsterVector, MyForwardVector);
	double Radian = FMath::Acos(DotProductValue);
	double Degree = FMath::RadiansToDegrees(Radian);

	LOG_MSG(TEXT("Kick Angle is %f"), Degree);

	if (Degree >= 0.0f && Degree < 45.0f)
	{
		KickSectionStr += TEXT("Front");
	}
	else if (Degree > 135.0f && Degree <= 180.0f)
	{
		KickSectionStr += TEXT("Back");
	}
	else
	{
		KickSectionStr += TEXT("Side");
	}

	//날아가는 각도
	FVector LaunchDirXY = CauserToMonsterVector;
	float LaunchZ = 1.0f;
	LaunchDirXY.Z = LaunchZ;

	FVector LaunchDir = FVector(LaunchDirXY.X, LaunchDirXY.Y, LaunchZ);
	LaunchDir.Normalize();

	//GetCharacterMovement()->AddImpulse(LaunchDir * 300.0f);
	LaunchCharacter(LaunchDir * 500.0f, true, true);

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	//기존 애니메이션 모두 중지
	AnimInstance->StopAllMontages(0.1f);
	AnimInstance->Montage_Play(DamagedMontage, 1.0f);
	AnimInstance->Montage_JumpToSection(FName(KickSectionStr + TEXT("Start")), DamagedMontage);
	AnimInstance->SetRootMotionMode(ERootMotionMode::NoRootMotionExtraction);

	TWeakObjectPtr<UAnimInstance> WeakAnimInstance = AnimInstance;

	OnLandedByKickJump.BindLambda([this, WeakAnimInstance, KickSectionStr]()
		{
			WeakAnimInstance.Get()->StopAllMontages(0.1f);
			WeakAnimInstance.Get()->Montage_Play(DamagedMontage, 1.0f);
			WeakAnimInstance.Get()->Montage_JumpToSection(FName(KickSectionStr + TEXT("End")), DamagedMontage);
			WeakAnimInstance.Get()->SetRootMotionMode(ERootMotionMode::RootMotionFromEverything);
		}
	);
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

	ParryTimeOff();
	SetCurrentState(EMonsterState::Groggy);
	
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	if (AnimInstance == nullptr)
	{
		LOG_WARNING(TEXT("AnimInstance is Nullptr"));
		return;
	}

	AnimInstance->Montage_Play(ParriedMontage, 1.0f);
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

void ABMonsterBase::KickJumpUpdate()
{
	if (GetCurrentState() == EMonsterState::Kicked)
	{
		if (GetCharacterMovement()->IsMovingOnGround() == false)
		{
			OnLandedByKickJump.ExecuteIfBound();
			OnLandedByKickJump.Unbind();
		}
	}
}
