// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Monster/MonsterActor/BBasicMonsterBase.h"
#include "../AIController/BAIBasicMonsterController.h"
#include "../DataAsset/BMonsterStatData.h"
#include "BiohazardRe4.h"

#include "DamageType/MonsterDamageType/BDMGMonsterLargeBottom.h"
#include "DamageType/MonsterDamageType/BDMGMonsterLargeTop.h"
#include "DamageType/MonsterDamageType/BDMGMonsterSmallBottom.h"
#include "DamageType/MonsterDamageType/BDMGMonsterSmallTop.h"

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

	//�̸�, ���
	FString Weapon_1 = TEXT("HandAxe");
	FString WeaponPath_1 = TEXT("/ Script / Engine.SkeletalMesh'/Game/Assets/Monster/Mesh/BasicMonster/Weapon/HandAxe/SK_HandAxe.SK_HandAxe'");

	FString Weapon_2 = TEXT("KitchenKnife");
	FString WeaponPath_2 = TEXT("/Script/Engine.SkeletalMesh'/Game/Assets/Monster/Mesh/BasicMonster/Weapon/KitchenKnife/SK_KitchenKnife.SK_KitchenKnife'");

	FString Weapon_3 = TEXT("Sickle");
	FString WeaponPath_3 = TEXT("/Script/Engine.SkeletalMesh'/Game/Assets/Monster/Mesh/BasicMonster/Weapon/Sickle/SK_Sickle.SK_Sickle'");

	FString Weapon_4 = TEXT("Torch");
	FString WeaponPath_4 = TEXT("/Script/Engine.SkeletalMesh'/Game/Assets/Monster/Mesh/BasicMonster/Weapon/Torch/SK_Torch.SK_Torch'");

	//FString Weapon_5 = TEXT("Rake");
	//FString WeaponPath_5 = TEXT("/Script/Engine.SkeletalMesh'/Game/Assets/Monster/Mesh/BasicMonster/Weapon/Rake/SK_Rake.SK_Rake'");


	TArray<TPair<FString, FString>> WeaponNameArray;
	WeaponNameArray.Reserve(5);

	WeaponNameArray.Add({ Weapon_1, WeaponPath_1 });
	WeaponNameArray.Add({ Weapon_2, WeaponPath_2 });
	WeaponNameArray.Add({ Weapon_3, WeaponPath_3 });
	WeaponNameArray.Add({ Weapon_4, WeaponPath_4 });
	//WeaponNameArray.Add({ Weapon_5, WeaponPath_5 });

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
