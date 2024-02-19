// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Monster/MonsterActor/BMonsterBase.h"
#include "AIController.h"
#include "Kismet/GameplayStatics.h"
#include "Components/CapsuleComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "BiohazardRe4.h"
#include "Actor/Monster/Define/MonsterDefine.h"
#include "Actor/Monster/DataAsset/BMonsterStatData.h"
#include "Actor/Monster/Component/BMonsterStatComponent.h"

// Sets default values
ABMonsterBase::ABMonsterBase()
{
	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationYaw = false;
	
	WeaponCollision = CreateDefaultSubobject<UCapsuleComponent>(TEXT("WeaponCollision"));
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
	
	AttackUpdate();
	BurstJumpUpdate();
	CrossWindowJumpUpdate();
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
		
	if (MyWeaponType == EWeaponType::None)
	{
		if (SectionNumber == 2)
		{
			WeaponCollision->AttachToComponent(GetMesh(), FAttachmentTransformRules(EAttachmentRule::KeepRelative, true), FName(TEXT("L_Palm")));
		}
		else
		{
			WeaponCollision->AttachToComponent(GetMesh(), FAttachmentTransformRules(EAttachmentRule::KeepRelative, true), FName(TEXT("R_Palm")));
		}
	}

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance == nullptr)
	{
		LOG_WARNING(TEXT("AnimInstance is Nullptr"));
		return;
	}

	AnimInstance->Montage_Play(AttackMontage);
	AnimInstance->Montage_JumpToSection(SectionName, AttackMontage);
}

void ABMonsterBase::AttackOn()
{
	bIsAttacking = true;
}

void ABMonsterBase::AttackOff()
{
	bIsAttacking = false;
}

void ABMonsterBase::AttackUpdate()
{
	if (bIsAttacking == false)
	{
		return;
	}

	TArray<AActor*> HitActors;
	WeaponCollision->GetOverlappingActors(HitActors, AActor::StaticClass());

	for (AActor* HitActor : HitActors)
	{
		ACharacter* HitCharacter = Cast<ACharacter>(HitActor);
		if (HitCharacter == nullptr)
		{
			continue;
		}

		if (HitCharacter->IsPlayerControlled() == true)
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
			
			bIsAttacking = false;
		}
	}
}

void ABMonsterBase::CrossWindow()
{
	GetCapsuleComponent()->SetCollisionProfileName(FName(TEXT("NoCollision")));
	GetMesh()->SetCollisionProfileName(FName(TEXT("NoCollision")));

	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Flying);

	SetCurrentState(EMonsterState::CrossWindowStart);
}

void ABMonsterBase::CrossWindowJumpStart()
{
	GetCapsuleComponent()->SetCollisionProfileName(FName(TEXT("MonsterCapsuleProfile")));
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Falling);

	OnLandedByCrossWindowJump.BindLambda(
		[this]() 
		{
			SetCurrentState(EMonsterState::CrossWindowEnd);
			GetMesh()->SetCollisionProfileName(FName(TEXT("MonsterCollision")));
		}
	);
}

void ABMonsterBase::CrossWindowJumpUpdate()
{
	if (GetCurrentState() == EMonsterState::CrossWindowStart && OnLandedByCrossWindowJump.IsBound() == true)
	{
		if (GetCharacterMovement()->IsFalling() == false)
		{
			CrossWindowJumpEnd();
		}
	}
}

void ABMonsterBase::CrossWindowJumpEnd()
{
	OnLandedByCrossWindowJump.ExecuteIfBound();
	OnLandedByCrossWindowJump.Unbind();
}

bool ABMonsterBase::isSetTargetInBlackBoard()
{
	AAIController* AIController = Cast<AAIController>(GetController());
	if (AIController == nullptr)
	{
		LOG_WARNING(TEXT("AIController is nullptr"));
		return 0.0f;
	}

	UObject* Target = AIController->GetBlackboardComponent()->GetValueAsObject(BBKEY_TARGET);

	if (Target != nullptr)
	{
		return true;
	}
	else
	{
		return false;
	}
}
