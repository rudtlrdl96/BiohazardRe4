// Fill out your copyright notice in the Description page of Project Settings.


#include "BMonsterMale.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "BiohazardRe4.h"
#include "Actor/Monster/Component/BMonsterStatComponent.h"
#include "Actor/Monster/Interface/BMonsterAnimInterface.h"

ABMonsterMale::ABMonsterMale()
{
	CreateComponent();
	SetSkeletalMeshInConstructor();

	InitAI();
	InitValue();

	static ConstructorHelpers::FClassFinder<UAnimInstance> AnimRef(TEXT("/Script/Engine.AnimBlueprint'/Game/Blueprints/Actor/Monster/ABP_BasicMonsterMale.ABP_BasicMonsterMale'"));
	if (AnimRef.Class != nullptr)
	{
		GetMesh()->SetAnimInstanceClass(AnimRef.Class);
	}

	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.TickInterval = 0.5f;
}

void ABMonsterMale::BeginPlay()
{
	Super::BeginPlay();
}

void ABMonsterMale::CreateComponent()
{
	BodyBase = GetMesh();
	
	//CreateComponent
	Head = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Head"));
	Jacket = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Jacket"));
	Pants = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Pants"));
	Hat = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Hat"));

	//Attach
	BodyBase->SetupAttachment(RootComponent);
	Head->SetupAttachment(RootComponent);
	Jacket->SetupAttachment(RootComponent);
	Pants->SetupAttachment(RootComponent);
	Hat->SetupAttachment(RootComponent);
}

void ABMonsterMale::InitValue()
{
	GetCharacterMovement()->MaxWalkSpeed = 50.0f;
}

void ABMonsterMale::SetAnimInstanceAndAnimationMontageInBeginPlay()
{
	if (MyWeaponType == EWeaponType::None)
	{
		FString AnimInstancePath = TEXT("/Script/Engine.AnimBlueprint'/Game/Blueprints/Actor/Monster/ABP_BasicMonsterMale.ABP_BasicMonsterMale'");
		UAnimBlueprint* AnimBlueprint = LoadObject<UAnimBlueprint>(nullptr, *AnimInstancePath);

		if (AnimBlueprint == nullptr)
		{
			LOG_MSG(TEXT("AnimBlueprint is nullptr"));
			return;
		}

		IBMonsterAnimInterface* AnimInterface = Cast<IBMonsterAnimInterface>(GetMesh()->GetAnimInstance());
		if (AnimInterface == nullptr)
		{
			LOG_MSG(TEXT("AnimInterface casting failed"));
			return;
		}
		
		AnimInterface->SetAnimationType(EMonsterAnimType::BareHands);

		FString AttackMontagePath = TEXT("/Script/Engine.AnimMontage'/Game/Blueprints/Actor/Monster/Animation/AM_MonsterMale_Attack_BareHands.AM_MonsterMale_Attack_BareHands'");
		AttackMontage = LoadObject<UAnimMontage>(nullptr, *AttackMontagePath);
		
		FString DamagedMontagePath = TEXT("/Script/Engine.AnimMontage'/Game/Blueprints/Actor/Monster/Animation/AM_MonsterMale_Damaged_BareHands.AM_MonsterMale_Damaged_BareHands'");
		DamagedMontage = LoadObject<UAnimMontage>(nullptr, *DamagedMontagePath);

		FString ParriedMontagePath = TEXT("/Script/Engine.AnimMontage'/Game/Blueprints/Actor/Monster/Animation/AM_MonsterMale_Parry_Bare_Hands.AM_MonsterMale_Parry_Bare_Hands'");
		ParriedMontage = LoadObject<UAnimMontage>(nullptr, *ParriedMontagePath);
	}
	else if (MyWeaponType == EWeaponType::OneHand)
	{
		FString AnimInstancePath = TEXT("/Script/Engine.AnimBlueprint'/Game/Blueprints/Actor/Monster/ABP_BasicMonsterMale.ABP_BasicMonsterMale'");
		UAnimBlueprint* AnimBlueprint = LoadObject<UAnimBlueprint>(nullptr, *AnimInstancePath);

		if (AnimBlueprint == nullptr)
		{
			LOG_MSG(TEXT("AnimBlueprint is nullptr"));
			return;
		}

		IBMonsterAnimInterface* AnimInterface = Cast<IBMonsterAnimInterface>(GetMesh()->GetAnimInstance());
		if (AnimInterface == nullptr)
		{
			LOG_MSG(TEXT("AnimInterface casting failed"));
			return;
		}
		
		AnimInterface->SetAnimationType(EMonsterAnimType::OneHand);
		
		FString AttackMontagePath = TEXT("/Script/Engine.AnimMontage'/Game/Blueprints/Actor/Monster/Animation/AM_MonsterMale_Attack_OneHand.AM_MonsterMale_Attack_OneHand'");
		AttackMontage = LoadObject<UAnimMontage>(nullptr, *AttackMontagePath);

		FString DamagedMontagePath = TEXT("/Script/Engine.AnimMontage'/Game/Blueprints/Actor/Monster/Animation/AM_MonsterMale_Damaged_OneHand.AM_MonsterMale_Damaged_OneHand'");
		DamagedMontage = LoadObject<UAnimMontage>(nullptr, *DamagedMontagePath);
	}
}

void ABMonsterMale::SetSkeletalMeshInConstructor()
{

}

void ABMonsterMale::Tick(float _DeltaTIme)
{
	Super::Tick(_DeltaTIme);

	IBMonsterAnimInterface* AnimInterface = Cast<IBMonsterAnimInterface>(GetMesh()->GetAnimInstance());
	if (AnimInterface == nullptr)
	{
		LOG_MSG(TEXT("AnimInterface casting failed"));
		return;
	}

	if (MyWeaponType == EWeaponType::None)
	{
		AnimInterface->SetAnimationType(EMonsterAnimType::BareHands);

	}
	else if (MyWeaponType == EWeaponType::OneHand)
	{
		AnimInterface->SetAnimationType(EMonsterAnimType::OneHand);
	}
}

void ABMonsterMale::SetClothesSkeletalMeshByRandomInBeginPlay()
{
	//Body
	FString BodyPath = TEXT("/Script/Engine.SkeletalMesh'/Game/Assets/Monster/Mesh/BasicMonster/Male/Base/SK_MonsterMaleBase.SK_MonsterMaleBase'");
	USkeletalMesh* LoadedBodyMesh = Cast<USkeletalMesh>(StaticLoadObject(USkeletalMesh::StaticClass(), nullptr, *BodyPath));

	GetMesh()->SetSkeletalMesh(LoadedBodyMesh);
	BodyBase = GetMesh();

	//Head, Hat
	FString HeadPath_1 = TEXT("/Script/Engine.SkeletalMesh'/Game/Assets/Monster/Mesh/BasicMonster/Male/Head/1/SK_MonsterMaleHead_1.SK_MonsterMaleHead_1'");
	FString HeadPath_2 = TEXT("/Script/Engine.SkeletalMesh'/Game/Assets/Monster/Mesh/BasicMonster/Male/Head/2/SK_MonsterMaleHead_2.SK_MonsterMaleHead_2'");
	FString HeadPath_3 = TEXT("/Script/Engine.SkeletalMesh'/Game/Assets/Monster/Mesh/BasicMonster/Male/Head/3/SK_MonsterMaleHead_3.SK_MonsterMaleHead_3'");
	FString HeadPath_4 = TEXT("/Script/Engine.SkeletalMesh'/Game/Assets/Monster/Mesh/BasicMonster/Male/Head/4/SK_MonsterMaleHead_4.SK_MonsterMaleHead_4'");

	FString HatPath_1 = TEXT("/Script/Engine.SkeletalMesh'/Game/Assets/Monster/Mesh/BasicMonster/Male/Hat/1/SK_MonsterMaleHat_1.SK_MonsterMaleHat_1'");
	FString HatPath_2 = TEXT("/Script/Engine.SkeletalMesh'/Game/Assets/Monster/Mesh/BasicMonster/Male/Hat/2/SK_MonsterMaleHat_2.SK_MonsterMaleHat_2'");
	FString HatPath_3 = TEXT("/Script/Engine.SkeletalMesh'/Game/Assets/Monster/Mesh/BasicMonster/Male/Hat/3/SK_MonsterMaleHat_3.SK_MonsterMaleHat_3'");

	//¸Ó¸®, ¸ðÀÚ ½Ö
	TArray<TPair<FString, FString>> HeadHatPaths;
	HeadHatPaths.Reserve(4);

	HeadHatPaths.Add({ HeadPath_1, HatPath_1 });
	HeadHatPaths.Add({ HeadPath_2, HatPath_2 });
	HeadHatPaths.Add({ HeadPath_3, HatPath_3 });
	HeadHatPaths.Add({ HeadPath_4, FString(TEXT(""))});

	int HeadHatIndex = FMath::RandRange(0, 3);

	USkeletalMesh* LoadedHeadMesh = Cast<USkeletalMesh>(StaticLoadObject(USkeletalMesh::StaticClass(), nullptr, *HeadHatPaths[HeadHatIndex].Key));
	if (LoadedHeadMesh == nullptr)
	{
		LOG_WARNING(TEXT("Head Mesh is failed to load"));
	}
	else
	{
		Head->SetSkeletalMesh(LoadedHeadMesh);
	}

	if (*HeadHatPaths[HeadHatIndex].Value == FString(TEXT("")))
	{
		Hat->SetVisibility(false);
	}
	else
	{
		USkeletalMesh* LoadedHatMesh = Cast<USkeletalMesh>(StaticLoadObject(USkeletalMesh::StaticClass(), nullptr, *HeadHatPaths[HeadHatIndex].Value));
		if (LoadedHatMesh == nullptr)
		{
			LOG_WARNING(TEXT("Hat Mesh is failed to load"));
		}
		else
		{
			Hat->SetSkeletalMesh(LoadedHatMesh);
		}
	}

	//Jacket
	FString JacketPath_1 = TEXT("/Script/Engine.SkeletalMesh'/Game/Assets/Monster/Mesh/BasicMonster/Male/Jacket/1/SK_MonsterMaleJacket_1.SK_MonsterMaleJacket_1'");
	FString JacketPath_2 = TEXT("/Script/Engine.SkeletalMesh'/Game/Assets/Monster/Mesh/BasicMonster/Male/Jacket/2/SK_MonsterMaleJacket_2.SK_MonsterMaleJacket_2'");
	FString JacketPath_3 = TEXT("/Script/Engine.SkeletalMesh'/Game/Assets/Monster/Mesh/BasicMonster/Male/Jacket/3/SK_MonsterMaleJacket_3.SK_MonsterMaleJacket_3'");
	FString JacketPath_4 = TEXT("/Script/Engine.SkeletalMesh'/Game/Assets/Monster/Mesh/BasicMonster/Male/Jacket/4/SK_MonsterMaleJacket_4.SK_MonsterMaleJacket_4'");

	TArray<FString> JacketPaths;
	JacketPaths.Reserve(4);
	
	JacketPaths.Add(JacketPath_1);
	JacketPaths.Add(JacketPath_2);
	JacketPaths.Add(JacketPath_3);
	JacketPaths.Add(JacketPath_4);
	
	int JacketIndex = FMath::RandRange(0, 3);
	USkeletalMesh* LoadedJacketMesh = Cast<USkeletalMesh>(StaticLoadObject(USkeletalMesh::StaticClass(), nullptr, *JacketPaths[JacketIndex]));

	if (LoadedJacketMesh != nullptr)
	{
		Jacket->SetSkeletalMesh(LoadedJacketMesh);
	}
	else
	{
		LOG_MSG(TEXT("Jacket Mesh is Nullptr : Index = %d"), JacketIndex);
	}
	 
	//Pants
	FString PantsPath_1 = TEXT("/Script/Engine.SkeletalMesh'/Game/Assets/Monster/Mesh/BasicMonster/Male/Pants/1/SK_MonsterMalePants_1.SK_MonsterMalePants_1'");
	FString PantsPath_2 = TEXT("/Script/Engine.SkeletalMesh'/Game/Assets/Monster/Mesh/BasicMonster/Male/Pants/2/SK_MonsterMalePants_2.SK_MonsterMalePants_2'");
	FString PantsPath_3 = TEXT("/Script/Engine.SkeletalMesh'/Game/Assets/Monster/Mesh/BasicMonster/Male/Pants/3/SK_MonsterMalePants_3.SK_MonsterMalePants_3'");

	TArray<FString> PantsRefs;
	PantsRefs.Reserve(3);

	PantsRefs.Add(PantsPath_1);
	PantsRefs.Add(PantsPath_2);
	PantsRefs.Add(PantsPath_3);

	int PantsIndex = FMath::RandRange(0, 2);

	USkeletalMesh* LoadedPantsMesh = Cast<USkeletalMesh>(StaticLoadObject(USkeletalMesh::StaticClass(), nullptr, *PantsRefs[PantsIndex]));

	if (LoadedPantsMesh != nullptr)
	{
		Pants->SetSkeletalMesh(LoadedPantsMesh);
	}
	else
	{
		LOG_MSG(TEXT("Pants Mesh is Nullptr : Index = %d"), PantsIndex);
	}
}

