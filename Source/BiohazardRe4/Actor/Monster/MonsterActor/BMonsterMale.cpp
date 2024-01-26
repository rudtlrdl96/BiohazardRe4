// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Monster/MonsterActor/BMonsterMale.h"
#include "BiohazardRe4.h"
#include "../AIController/BAIBasicMonsterController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "../Component/BMonsterStatComponent.h"

ABMonsterMale::ABMonsterMale()
{
	CreateComponent();
	SetSkeletalMeshInConstructor();

	InitAI();
	InitValue();
}

void ABMonsterMale::BeginPlay()
{
	Super::BeginPlay();

	SetSkeletalMeshByRandomInBeginPlay();
}

void ABMonsterMale::CreateComponent()
{
	//CreateComponent
	BodyBase = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("BodyBase"));
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

void ABMonsterMale::InitAI()
{
	AIControllerClass = ABAIBasicMonsterController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

void ABMonsterMale::InitValue()
{
	GetCharacterMovement()->MaxWalkSpeed = 50.0f;
	Stat->SetAttackRadius(80.0f);
}

void ABMonsterMale::SetSkeletalMeshInConstructor()
{
	//BaseBody
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> BodyBaseRef(TEXT("/Script/Engine.SkeletalMesh'/Game/Assets/Monster/Mesh/BasicMonster/Male/Base/SK_MonsterMaleBase.SK_MonsterMaleBase'"));
	
	if(BodyBaseRef.Object != nullptr)
	{
		BodyBase->SetSkeletalMesh(BodyBaseRef.Object);
	}
	else
	{
		LOG_MSG(TEXT("Body Mesh is Nullptr"));
	}
	
}

void ABMonsterMale::SetSkeletalMeshByRandomInBeginPlay()
{
	//Head
	FString HeadPath_1 = TEXT("/Script/Engine.SkeletalMesh'/Game/Assets/Monster/Mesh/BasicMonster/Male/Head/1/SK_MonsterMaleHead_1.SK_MonsterMaleHead_1'");
	FString HeadPath_2 = TEXT("/Script/Engine.SkeletalMesh'/Game/Assets/Monster/Mesh/BasicMonster/Male/Head/2/SK_MonsterMaleHead_2.SK_MonsterMaleHead_2'");
	FString HeadPath_3 = TEXT("/Script/Engine.SkeletalMesh'/Game/Assets/Monster/Mesh/BasicMonster/Male/Head/3/SK_MonsterMaleHead_3.SK_MonsterMaleHead_3'");
	FString HeadPath_4 = TEXT("/Script/Engine.SkeletalMesh'/Game/Assets/Monster/Mesh/BasicMonster/Male/Head/4/SK_MonsterMaleHead_4.SK_MonsterMaleHead_4'");

	TArray<FString> HeadPaths;
	HeadPaths.Reserve(4);

	HeadPaths.Add(HeadPath_1);
	HeadPaths.Add(HeadPath_2);
	HeadPaths.Add(HeadPath_3);
	HeadPaths.Add(HeadPath_4);

	int HeadIndex = FMath::RandRange(0, 3);

	USkeletalMesh* LoadedHeadMesh = Cast<USkeletalMesh>(StaticLoadObject(USkeletalMesh::StaticClass(), nullptr, *HeadPaths[HeadIndex]));

	if (LoadedHeadMesh != nullptr)
	{
		Head->SetSkeletalMesh(LoadedHeadMesh);
	}
	else
	{
		LOG_MSG(TEXT("Head Mesh is Nullptr : Index = %d"), HeadIndex);
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

	//Hat
	int HatIndex = FMath::RandRange(0, 3);
	if (HatIndex == 3)
	{
		Hat->SetVisibility(false);
		return;
	}

	FString HatPath_1 = TEXT("/Script/Engine.SkeletalMesh'/Game/Assets/Monster/Mesh/BasicMonster/Male/Hat/1/SK_MonsterMaleHat_1.SK_MonsterMaleHat_1'");
	FString HatPath_2 = TEXT("/Script/Engine.SkeletalMesh'/Game/Assets/Monster/Mesh/BasicMonster/Male/Hat/2/SK_MonsterMaleHat_2.SK_MonsterMaleHat_2'");
	FString HatPath_3 = TEXT("/Script/Engine.SkeletalMesh'/Game/Assets/Monster/Mesh/BasicMonster/Male/Hat/3/SK_MonsterMaleHat_3.SK_MonsterMaleHat_3'");

	TArray<FString> HatRefs;
	HatRefs.Reserve(3);

	HatRefs.Add(HatPath_1);
	HatRefs.Add(HatPath_2);
	HatRefs.Add(HatPath_3);

	USkeletalMesh* LoadedHatMesh = Cast<USkeletalMesh>(StaticLoadObject(USkeletalMesh::StaticClass(), nullptr, *HatRefs[HatIndex]));

	if (LoadedHatMesh != nullptr)
	{
		Hat->SetSkeletalMesh(LoadedHatMesh);
	}
	else
	{
		LOG_MSG(TEXT("Hat Mesh is Nullptr : Index = %d"), HatIndex);
	}
}

