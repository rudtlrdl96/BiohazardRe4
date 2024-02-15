// Fill out your copyright notice in the Description page of Project Settings.


#include "BBasicMonsterBase.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BiohazardRe4.h"

#include "DamageType/MonsterDamageType/BDMGMonsterSmallTop.h"
#include "DamageType/MonsterDamageType/BDMGMonsterLargeTop.h"
#include "DamageType/MonsterDamageType/BDMGMonsterSmallBottom.h"
#include "DamageType/MonsterDamageType/BDMGMonsterLargeBottom.h"

#include "Actor/Monster/Define/MonsterDefine.h"
#include "Actor/Monster/DataAsset/BMonsterStatData.h"
#include "Actor/Monster/Interface/BMonsterAnimInterface.h"
#include "Actor/Monster/AIController/BAIBasicMonsterController.h"

ABBasicMonsterBase::ABBasicMonsterBase()
{
	Weapon = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Weapon"));
	
	static ConstructorHelpers::FObjectFinder<UBMonsterStatData> BasicMonsterStatDataRef(TEXT("/Script/BiohazardRe4.BMonsterStatData'/Game/Blueprints/Actor/Monster/DataAsset/DA_BasicMonsterStat.DA_BasicMonsterStat'"));

	if (BasicMonsterStatDataRef.Object != nullptr)
	{
		StatInit(BasicMonsterStatDataRef.Object);
	}
}

void ABBasicMonsterBase::BeginPlay()
{
	Super::BeginPlay();

	SetlMeshAndAnimationByRandomInBeginPlay();
	InitDamageTypes();
}

void ABBasicMonsterBase::SetlMeshAndAnimationByRandomInBeginPlay()
{
	SetClothesSkeletalMeshByRandomInBeginPlay();
	SetWeaponSkeletalMeshByRandomInBeginPlay();
	SetAnimInstanceAndAnimationMontageInBeginPlay();
}

void ABBasicMonsterBase::SetWeaponSkeletalMeshByRandomInBeginPlay()
{
	//Weapon
	int WeaponIndex = FMath::RandRange(0, 4);
	if (WeaponIndex == 4)
	{
		LOG_MSG(TEXT("Monster Bare Hand"));
		Weapon->SetVisibility(false);
		MyWeaponType = EWeaponType::None;

		return;
	}
	else
	{
		LOG_MSG(TEXT("Monster One Hand"));
		MyWeaponType = EWeaponType::OneHand;
	}

	//이름, 경로
	FString Weapon_1 = TEXT("HandAxe");
	FString WeaponPath_1 = TEXT("/ Script / Engine.SkeletalMesh'/Game/Assets/Monster/Mesh/BasicMonster/Weapon/HandAxe/SK_HandAxe.SK_HandAxe'");

	FString Weapon_2 = TEXT("KitchenKnife");
	FString WeaponPath_2 = TEXT("/Script/Engine.SkeletalMesh'/Game/Assets/Monster/Mesh/BasicMonster/Weapon/KitchenKnife/SK_KitchenKnife.SK_KitchenKnife'");

	FString Weapon_3 = TEXT("Sickle");
	FString WeaponPath_3 = TEXT("/Script/Engine.SkeletalMesh'/Game/Assets/Monster/Mesh/BasicMonster/Weapon/Sickle/SK_Sickle.SK_Sickle'");

	FString Weapon_4 = TEXT("Torch");
	FString WeaponPath_4 = TEXT("/Script/Engine.SkeletalMesh'/Game/Assets/Monster/Mesh/BasicMonster/Weapon/Torch/SK_Torch.SK_Torch'");


	TArray<TPair<FString, FString>> WeaponNameArray;
	WeaponNameArray.Reserve(5);

	WeaponNameArray.Add({ Weapon_1, WeaponPath_1 });
	WeaponNameArray.Add({ Weapon_2, WeaponPath_2 });
	WeaponNameArray.Add({ Weapon_3, WeaponPath_3 });
	WeaponNameArray.Add({ Weapon_4, WeaponPath_4 });

	USkeletalMesh* LoadedWeaponMesh = Cast<USkeletalMesh>(StaticLoadObject(USkeletalMesh::StaticClass(), nullptr, *WeaponNameArray[WeaponIndex].Value));
	
	FString SocketString = WeaponNameArray[WeaponIndex].Key + "Socket";
	FName SocketName = FName(SocketString);
	
	if (LoadedWeaponMesh == nullptr)
	{
		LOG_WARNING(TEXT("WeaponMesh is nullptr"));
	}

	if (BodyBase == nullptr)
	{
		LOG_WARNING(TEXT("BodyBase is nullptr"));
	}

	Weapon->SetSkeletalMesh(LoadedWeaponMesh);
	Weapon->AttachToComponent(GetMesh(), FAttachmentTransformRules(EAttachmentRule::KeepRelative, true), SocketName);
	
}

void ABBasicMonsterBase::InitAI()
{
	AIControllerClass = ABAIBasicMonsterController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

void ABBasicMonsterBase::SetClothesSkeletalMeshByRandomInBeginPlay()
{

}

void ABBasicMonsterBase::SetAnimInstanceAndAnimationMontageInBeginPlay()
{

}

void ABBasicMonsterBase::InitDamageTypes()
{
	DamageTypes.Reserve(10);

	if (MyWeaponType == EWeaponType::None)
	{
		//Attack1
		DamageTypes.Add(UBDMGMonsterSmallTop::StaticClass());

		//Attack2
		DamageTypes.Add(UBDMGMonsterLargeTop::StaticClass());
	}
	else if (MyWeaponType == EWeaponType::OneHand)
	{
		//Attack1
		DamageTypes.Add(UBDMGMonsterSmallTop::StaticClass());

		//Attack2
		DamageTypes.Add(UBDMGMonsterSmallTop::StaticClass());
	}
}

void ABBasicMonsterBase::SetDamagedSectionNums()
{
	if(MyWeaponType == EWeaponType::None)
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
			DamagedMontageSectionNums[TEXT("BODY")].Add(TEXT("MEDIUM"), 3);
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
			DamagedMontageSectionNums[TEXT("RARM")].Add(TEXT("MEDIUM"), 1);
		}

		//LReg
		{
			TMap<FString, int32> Maps;
			DamagedMontageSectionNums.Add(TEXT("LLEG"), Maps);
			DamagedMontageSectionNums[TEXT("LLEG")].Add(TEXT("SMALL"), 1);
			DamagedMontageSectionNums[TEXT("LLEG")].Add(TEXT("MEDIUM"), 1);
			DamagedMontageSectionNums[TEXT("LLEG")].Add(TEXT("LARGE"), 2);
		}

		//RReg
		{
			TMap<FString, int32> Maps;
			DamagedMontageSectionNums.Add(TEXT("RLEG"), Maps);
			DamagedMontageSectionNums[TEXT("RLEG")].Add(TEXT("SMALL"), 1);
			DamagedMontageSectionNums[TEXT("RLEG")].Add(TEXT("MEDIUM"), 1);
			DamagedMontageSectionNums[TEXT("RLEG")].Add(TEXT("LARGE"), 2);
		}
	}
	else if (MyWeaponType == EWeaponType::OneHand)
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
			DamagedMontageSectionNums[TEXT("BODY")].Add(TEXT("MEDIUM"), 3);
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
			DamagedMontageSectionNums[TEXT("RARM")].Add(TEXT("MEDIUM"), 1);
		}

		//LReg
		{
			TMap<FString, int32> Maps;
			DamagedMontageSectionNums.Add(TEXT("LLEG"), Maps);
			DamagedMontageSectionNums[TEXT("LLEG")].Add(TEXT("SMALL"), 1);
			DamagedMontageSectionNums[TEXT("LLEG")].Add(TEXT("MEDIUM"), 1);
			DamagedMontageSectionNums[TEXT("LLEG")].Add(TEXT("LARGE"), 2);
		}

		//RReg
		{
			TMap<FString, int32> Maps;
			DamagedMontageSectionNums.Add(TEXT("RLEG"), Maps);
			DamagedMontageSectionNums[TEXT("RLEG")].Add(TEXT("SMALL"), 1);
			DamagedMontageSectionNums[TEXT("RLEG")].Add(TEXT("MEDIUM"), 1);
			DamagedMontageSectionNums[TEXT("RLEG")].Add(TEXT("LARGE"), 2);
		}
	}
}

float ABBasicMonsterBase::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float ResultDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	
	//행동트리
	AAIController* AIController = Cast<AAIController>(GetController());
	if (AIController == nullptr)
	{
		LOG_WARNING(TEXT("AIController is nullptr"));
		return 0.0f;
	}

	AController* TargetController = GetWorld()->GetFirstPlayerController();
	if (TargetController == nullptr)
	{
		LOG_WARNING(TEXT("PlayerController is nullptr"));
		return 0.0f;
	}

	APawn* TargetPawn = TargetController->GetPawn();
	if (TargetPawn == nullptr)
	{
		LOG_WARNING(TEXT("PlayerPawn is nullptr"));
		return 0.0f;
	}

	AIController->GetBlackboardComponent()->SetValueAsBool(BBKEY_ISDETECTED, true);
	AIController->GetBlackboardComponent()->SetValueAsObject(BBKEY_TARGET, TargetPawn);

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance == nullptr)
	{
		LOG_WARNING(TEXT("AnimInstance is nullptr"));
		return 0.0f;
	}

	IBMonsterAnimInterface* AnimInterface = Cast<IBMonsterAnimInterface>(AnimInstance);
	if (AnimInterface == nullptr)
	{
		LOG_WARNING(TEXT("AnimInterface is nullptr"));
		return 0.0f;
	}

	AnimInterface->SetTarget(TargetPawn);

	return ResultDamage;
}
