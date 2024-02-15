// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Monster/MonsterActor/ChainsawMan/BChainsawMan.h"

#include "Engine/DamageEvents.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "DamageType/MonsterDamageType/BDMGMonsterSmallTop.h"
#include "DamageType/MonsterDamageType/BDMGMonsterLargeTop.h"
#include "DamageType/MonsterDamageType/BDMGMonsterSmallBottom.h"
#include "DamageType/MonsterDamageType/BDMGMonsterLargeBottom.h"

#include "BiohazardRe4.h"
#include "Actor/Monster/DataAsset/BMonsterStatData.h"
#include "Actor/Monster/Component/BMonsterStatComponent.h"
#include "Actor/Monster/AIController/BAIChainsawManController.h"

ABChainsawMan::ABChainsawMan()
{
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> ChainsawSkeletalMeshRef(TEXT("/Script/Engine.SkeletalMesh'/Game/Assets/Monster/Mesh/ChainsawMan/Chainsaw/SK_Chainsaw.SK_Chainsaw'"));

	Weapon = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SK_Chainsaw"));
	Weapon->SetupAttachment(GetMesh(), TEXT("ChainsawSocket"));
	
	if (ChainsawSkeletalMeshRef.Object != nullptr)
	{
		Weapon->SetSkeletalMesh(ChainsawSkeletalMeshRef.Object);
		Weapon->SetCollisionProfileName(TEXT("NoCollision"));
	}

	MyWeaponType = EWeaponType::TwoHands;

	static ConstructorHelpers::FObjectFinder<UAnimMontage> ChainsawAttackMontageRef(TEXT("/Script/Engine.AnimMontage'/Game/Blueprints/Actor/Monster/Animation/AM_ChainsawManAttack.AM_ChainsawManAttack'"));
	
	if (ChainsawAttackMontageRef.Object != nullptr)
	{
		AttackMontage = ChainsawAttackMontageRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> ChainsawDamagedMontageRef(TEXT("/Script/Engine.AnimMontage'/Game/Blueprints/Actor/Monster/Animation/AM_ChainsawManDamaged.AM_ChainsawManDamaged'"));

	if (ChainsawDamagedMontageRef.Object != nullptr)
	{
		DamagedMontage = ChainsawDamagedMontageRef.Object;
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

	InitDamageTypes();
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

	WeaponCollision->AttachToComponent(Weapon, FAttachmentTransformRules(EAttachmentRule::KeepRelative, true), FName(TEXT("CollisionSocket")));
	WeaponCollision->SetCapsuleSize(25.0f, 50.0f);
}

void ABChainsawMan::MonsterDeathByPoint(const FDamageEvent& _DamageEvent)
{
	//행동트리
	AAIController* AIController = Cast<AAIController>(GetController());
	if (AIController == nullptr)
	{
		LOG_WARNING(TEXT("AIController is nullptr"));
		return;
	}

	AIController->UnPossess();

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	int SectionIndex = FMath::RandRange(1, 3);

	FString SectionNameStr = TEXT("Death");
	SectionNameStr += FString::FromInt(SectionIndex);

	FName SectionName(SectionNameStr);

	//기존 애니메이션 모두 중지
	AnimInstance->StopAllMontages(0.1f);

	AnimInstance->Montage_Play(DamagedMontage, 1.0f);
	AnimInstance->Montage_JumpToSection(SectionName, DamagedMontage);

	SetCurrentState(EMonsterState::Death);

	DamagedMontage->bEnableAutoBlendOut = false;

	GetMesh()->SetCollisionProfileName(TEXT("NoCollision"));
}

void ABChainsawMan::MonsterDeathByNormal(const FDamageEvent& _DamageEvent, const AActor* DamageCauser)
{
	//Super::MonsterDeathByNormal(_DamageEvent, DamageCauser);
}

void ABChainsawMan::InitDamageTypes()
{
	DamageTypes.Reserve(10);

	//Attack1
	DamageTypes.Emplace(UBDMGMonsterLargeTop::StaticClass());
	
	//Attack2
	DamageTypes.Emplace(UBDMGMonsterLargeBottom::StaticClass());
}

void ABChainsawMan::SetDamagedSectionNums()
{
	//Head
	{
		TMap<FString, int32> Maps;
		DamagedMontageSectionNums.Add(TEXT("HEAD"), Maps);
		DamagedMontageSectionNums[TEXT("HEAD")].Add(TEXT("SMALL"), 3);
	}

	//Body
	{
		TMap<FString, int32> Maps;
		DamagedMontageSectionNums.Add(TEXT("BODY"), Maps);
		DamagedMontageSectionNums[TEXT("BODY")].Add(TEXT("SMALL"), 3);
		DamagedMontageSectionNums[TEXT("BODY")].Add(TEXT("MEDIUM"), 1);
	}

	//LArm
	{
		TMap<FString, int32> Maps;
		DamagedMontageSectionNums.Add(TEXT("LARM"), Maps);
		DamagedMontageSectionNums[TEXT("LARM")].Add(TEXT("SMALL"), 1);
		DamagedMontageSectionNums[TEXT("LARM")].Add(TEXT("MEDIUM"), 1);
	}

	//RArm
	{
		TMap<FString, int32> Maps;
		DamagedMontageSectionNums.Add(TEXT("RARM"), Maps);
		DamagedMontageSectionNums[TEXT("RARM")].Add(TEXT("SMALL"), 1);
	}

	//LReg
	{
		TMap<FString, int32> Maps;
		DamagedMontageSectionNums.Add(TEXT("LLEG"), Maps);
		DamagedMontageSectionNums[TEXT("LLEG")].Add(TEXT("SMALL"), 1);
		DamagedMontageSectionNums[TEXT("LLEG")].Add(TEXT("MEDIUM"), 1);
	}

	//RReg
	{
		TMap<FString, int32> Maps;
		DamagedMontageSectionNums.Add(TEXT("RLEG"), Maps);
		DamagedMontageSectionNums[TEXT("RLEG")].Add(TEXT("SMALL"), 1);
		DamagedMontageSectionNums[TEXT("RLEG")].Add(TEXT("MEDIUM"), 1);
	}
}


