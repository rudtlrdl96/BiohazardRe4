// Fill out your copyright notice in the Description page of Project Settings.


#include "BMonsterFemale.h"
#include "BiohazardRe4.h"
#include "../AIController/BAIBasicMonsterController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "../Component/BMonsterStatComponent.h"

ABMonsterFemale::ABMonsterFemale()
{
	CreateComponent();
	SetSkeletalMeshInConstructor();

	InitAI();
	InitValue();
}

void ABMonsterFemale::BeginPlay()
{
	Super::BeginPlay();

	SetSkeletalMeshByRandomInBeginPlay();
}

void ABMonsterFemale::CreateComponent()
{
	//CreateComponent
	BodyBase = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("BodyBase"));
	Head = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Head"));
	Shirt = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Jacket"));
	Pants = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Pants"));
	Hat = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Hat"));

	//Attach
	BodyBase->SetupAttachment(RootComponent);
	Head->SetupAttachment(RootComponent);
	Shirt->SetupAttachment(RootComponent);
	Pants->SetupAttachment(RootComponent);
	Hat->SetupAttachment(RootComponent);
}

void ABMonsterFemale::InitAI()
{
	AIControllerClass = ABAIBasicMonsterController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

void ABMonsterFemale::InitValue()
{
	GetCharacterMovement()->MaxWalkSpeed = 50.0f;
}

void ABMonsterFemale::SetSkeletalMeshInConstructor()
{
	//BaseBody
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> BodyBaseRef(TEXT("/Script/Engine.SkeletalMesh'/Game/Assets/Monster/Mesh/BasicMonster/Female/Base/SK_MonsterFemaleBase.SK_MonsterFemaleBase'"));

	if (BodyBaseRef.Object != nullptr)
	{
		BodyBase->SetSkeletalMesh(BodyBaseRef.Object);
	}
	else
	{
		LOG_MSG(TEXT("Body Mesh is Nullptr"));
	}
}

void ABMonsterFemale::SetSkeletalMeshByRandomInBeginPlay()
{
	//Head
	FString HeadPath_1 = TEXT("/Script/Engine.SkeletalMesh'/Game/Assets/Monster/Mesh/BasicMonster/Female/Head/1/SK_MonsterFemaleHead_1.SK_MonsterFemaleHead_1'");
	FString HeadPath_2 = TEXT("/Script/Engine.SkeletalMesh'/Game/Assets/Monster/Mesh/BasicMonster/Female/Head/2/SK_MonsterFemaleHead_2.SK_MonsterFemaleHead_2'");
	FString HeadPath_3 = TEXT("/Script/Engine.SkeletalMesh'/Game/Assets/Monster/Mesh/BasicMonster/Female/Head/3/SK_MonsterFemaleHead_3.SK_MonsterFemaleHead_3'");

	TArray<FString> HeadPaths;
	HeadPaths.Reserve(3);

	HeadPaths.Add(HeadPath_1);
	HeadPaths.Add(HeadPath_2);
	HeadPaths.Add(HeadPath_3);

	int HeadIndex = FMath::RandRange(0, 2);

	USkeletalMesh* LoadedHeadMesh = Cast<USkeletalMesh>(StaticLoadObject(USkeletalMesh::StaticClass(), nullptr, *HeadPaths[HeadIndex]));

	if (LoadedHeadMesh != nullptr)
	{
		Head->SetSkeletalMesh(LoadedHeadMesh);
	}
	else
	{
		LOG_MSG(TEXT("Head Mesh is Nullptr : Index = %d"), HeadIndex);
	}

	//Shirt
	FString ShirtPath_1 = TEXT("/Script/Engine.SkeletalMesh'/Game/Assets/Monster/Mesh/BasicMonster/Female/Shirt/1/SK_MonsterFemaleShirt_1.SK_MonsterFemaleShirt_1'");
	FString ShirtPath_2 = TEXT("/Script/Engine.SkeletalMesh'/Game/Assets/Monster/Mesh/BasicMonster/Female/Shirt/2/SK_MonsterFemaleShirt_2.SK_MonsterFemaleShirt_2'");
	FString ShirtPath_3 = TEXT("/Script/Engine.SkeletalMesh'/Game/Assets/Monster/Mesh/BasicMonster/Female/Shirt/3/SK_MonsterFemaleShirt_3.SK_MonsterFemaleShirt_3'");
	FString ShirtPath_4 = TEXT("/Script/Engine.SkeletalMesh'/Game/Assets/Monster/Mesh/BasicMonster/Female/Shirt/4/SK_MonsterFemaleShirt_4.SK_MonsterFemaleShirt_4'");

	TArray<FString> ShirtPaths;
	ShirtPaths.Reserve(4);

	ShirtPaths.Add(ShirtPath_1);
	ShirtPaths.Add(ShirtPath_2);
	ShirtPaths.Add(ShirtPath_3);
	ShirtPaths.Add(ShirtPath_4);

	int ShirtIndex = FMath::RandRange(0, 3);
	USkeletalMesh* LoadedShirtMesh = Cast<USkeletalMesh>(StaticLoadObject(USkeletalMesh::StaticClass(), nullptr, *ShirtPaths[ShirtIndex]));

	if (LoadedShirtMesh != nullptr)
	{
		Shirt->SetSkeletalMesh(LoadedShirtMesh);
	}
	else
	{
		LOG_MSG(TEXT("Shirt Mesh is Nullptr : Index = %d"), ShirtIndex);
	}

	//Pants
	FString PantsPath_1 = TEXT("/Script/Engine.SkeletalMesh'/Game/Assets/Monster/Mesh/BasicMonster/Female/Pants/1/SK_MonsterFemalePants_1.SK_MonsterFemalePants_1'");
	FString PantsPath_2 = TEXT("/Script/Engine.SkeletalMesh'/Game/Assets/Monster/Mesh/BasicMonster/Female/Pants/2/SK_MonsterFemalePants_2.SK_MonsterFemalePants_2'");

	TArray<FString> PantsRefs;
	PantsRefs.Reserve(2);

	PantsRefs.Add(PantsPath_1);
	PantsRefs.Add(PantsPath_2);

	int PantsIndex = FMath::RandRange(0, 1);

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
	int HatIndex = FMath::RandRange(0, 1);
	if (HatIndex == 1)
	{
		Hat->SetVisibility(false);
		return;
	}

	FString HatPath_1 = TEXT("/Script/Engine.SkeletalMesh'/Game/Assets/Monster/Mesh/BasicMonster/Female/Hat/1/SK_MonsterFemaleHat_1.SK_MonsterFemaleHat_1'");
	USkeletalMesh* LoadedHatMesh = Cast<USkeletalMesh>(StaticLoadObject(USkeletalMesh::StaticClass(), nullptr, *HatPath_1));

	if (LoadedHatMesh != nullptr)
	{
		Hat->SetSkeletalMesh(LoadedHatMesh);
	}
	else
	{
		LOG_MSG(TEXT("Hat Mesh is Nullptr : Index = %d"), HatIndex);
	}
}
