// Fill out your copyright notice in the Description page of Project Settings.


#include "BMonsterFemale.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"

#include "BiohazardRe4.h"
#include "Actor/Monster/AIController/BAIBasicMonsterController.h"
#include "Actor/Monster/Component/BMonsterStatComponent.h"
#include "Actor/Monster/Interface/BMonsterAnimInterface.h"

TMap<EMeshType, TArray<TObjectPtr<USkeletalMesh>>> ABMonsterFemale::LoadedMesh;

ABMonsterFemale::ABMonsterFemale()
{
	CreateComponent();
	MeshLoad();

	CreateDamagedCollisionComponent();
	AttachDamagedCollisionComponentToMesh();
}

void ABMonsterFemale::BeginPlay()
{
	Super::BeginPlay();
}

void ABMonsterFemale::CreateComponent()
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

void ABMonsterFemale::MeshLoad()
{
	//BaseBody
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> BodyBaseRef(TEXT("/Script/Engine.SkeletalMesh'/Game/Assets/Monster/Mesh/BasicMonster/Female/Base/SK_MonsterFemaleBase.SK_MonsterFemaleBase'"));
	if (BodyBaseRef.Object != nullptr)
	{
		GetMesh()->SetSkeletalMesh(BodyBaseRef.Object);
		BodyBase = GetMesh();
	}

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> Head1Ref(TEXT("/Script/Engine.SkeletalMesh'/Game/Assets/Monster/Mesh/BasicMonster/Female/Head/1/SK_MonsterFemaleHead_1.SK_MonsterFemaleHead_1'"));
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> Head2Ref(TEXT("/Script/Engine.SkeletalMesh'/Game/Assets/Monster/Mesh/BasicMonster/Female/Head/2/SK_MonsterFemaleHead_2.SK_MonsterFemaleHead_2'"));
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> Head3Ref(TEXT("/Script/Engine.SkeletalMesh'/Game/Assets/Monster/Mesh/BasicMonster/Female/Head/3/SK_MonsterFemaleHead_3.SK_MonsterFemaleHead_3'"));

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> Hat1Ref(TEXT("/Script/Engine.SkeletalMesh'/Game/Assets/Monster/Mesh/BasicMonster/Female/Hat/1/SK_MonsterFemaleHat_1.SK_MonsterFemaleHat_1'"));

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> Jacket1Ref(TEXT("/Script/Engine.SkeletalMesh'/Game/Assets/Monster/Mesh/BasicMonster/Female/Shirt/1/SK_MonsterFemaleShirt_1.SK_MonsterFemaleShirt_1'"));
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> Jacket2Ref(TEXT("/Script/Engine.SkeletalMesh'/Game/Assets/Monster/Mesh/BasicMonster/Female/Shirt/2/SK_MonsterFemaleShirt_2.SK_MonsterFemaleShirt_2'"));
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> Jacket3Ref(TEXT("/Script/Engine.SkeletalMesh'/Game/Assets/Monster/Mesh/BasicMonster/Female/Shirt/3/SK_MonsterFemaleShirt_3.SK_MonsterFemaleShirt_3'"));

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> Pants1Ref(TEXT("/Script/Engine.SkeletalMesh'/Game/Assets/Monster/Mesh/BasicMonster/Female/Pants/1/SK_MonsterFemalePants_1.SK_MonsterFemalePants_1'"));
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> Pants2Ref(TEXT("/Script/Engine.SkeletalMesh'/Game/Assets/Monster/Mesh/BasicMonster/Female/Pants/2/SK_MonsterFemalePants_2.SK_MonsterFemalePants_2'"));

	if (0 == LoadedMesh.Num())
	{
		LoadedMesh.Add(EMeshType::Hat, TArray<TObjectPtr<class USkeletalMesh>>());
		LoadedMesh.Add(EMeshType::Head, TArray<TObjectPtr<class USkeletalMesh>>());
		LoadedMesh.Add(EMeshType::Jacket, TArray<TObjectPtr<class USkeletalMesh>>());
		LoadedMesh.Add(EMeshType::Pants, TArray<TObjectPtr<class USkeletalMesh>>());

		LoadedMesh[EMeshType::Head].Add(Head1Ref.Object);
		LoadedMesh[EMeshType::Head].Add(Head2Ref.Object);
		LoadedMesh[EMeshType::Head].Add(Head3Ref.Object);

		LoadedMesh[EMeshType::Hat].Add(Hat1Ref.Object);

		LoadedMesh[EMeshType::Jacket].Add(Jacket1Ref.Object);
		LoadedMesh[EMeshType::Jacket].Add(Jacket2Ref.Object);
		LoadedMesh[EMeshType::Jacket].Add(Jacket3Ref.Object);

		LoadedMesh[EMeshType::Pants].Add(Pants1Ref.Object);
		LoadedMesh[EMeshType::Pants].Add(Pants2Ref.Object);
	}
}

void ABMonsterFemale::SetClothesSkeletalMeshByRandomInBeginPlay()
{
	//¸Ó¸®, ¸ðÀÚ ½Ö
	int HeadIndex = FMath::RandRange(0, 2);

	if (LoadedMesh[EMeshType::Head][HeadIndex] == nullptr)
	{
		LOG_FATAL(TEXT("Head Mesh is failed to load"));
		return;
	}
	else
	{
		Head->SetSkeletalMesh(LoadedMesh[EMeshType::Head][HeadIndex]);
	}

	int HatIndex = FMath::RandRange(0, 3);

	if (HatIndex != 0)
	{
		Hat->SetVisibility(false);
	}
	else
	{
		if (LoadedMesh[EMeshType::Hat][HatIndex] == nullptr)
		{
			LOG_FATAL(TEXT("Hat Mesh is failed to load"));
			return;
		}
		else
		{
			Hat->SetSkeletalMesh(LoadedMesh[EMeshType::Hat][HatIndex]);
		}
	}

	//Jacket
	int JacketIndex = FMath::RandRange(0, 2);

	if (LoadedMesh[EMeshType::Jacket][JacketIndex] == nullptr)
	{
		LOG_FATAL(TEXT("Jacket Mesh is failed to load"));
		return;
	}
	else
	{
		Jacket->SetSkeletalMesh(LoadedMesh[EMeshType::Jacket][JacketIndex]);
	}

	//Pants
	int PantsIndex = FMath::RandRange(0, 1);

	if (LoadedMesh[EMeshType::Pants][PantsIndex] == nullptr)
	{
		LOG_FATAL(TEXT("Pants Mesh is failed to load"));
		return;
	}
	else
	{
		Pants->SetSkeletalMesh(LoadedMesh[EMeshType::Pants][PantsIndex]);
	}
}

void ABMonsterFemale::SetAnimInstanceAndAnimationMontageInBeginPlay()
{
	if (LoadedAnimInstance == nullptr)
	{
		//LOG_MSG(TEXT("Anim Instance is NullPtr"));
	}

	GetMesh()->SetAnimInstanceClass(LoadedAnimInstance);

	Head->SetMasterPoseComponent(GetMesh());
	Hat->SetMasterPoseComponent(GetMesh());
	Jacket->SetMasterPoseComponent(GetMesh());
	Pants->SetMasterPoseComponent(GetMesh());

	if (MyWeaponType == EWeaponType::None)
	{
		IBMonsterAnimInterface* AnimInterface = Cast<IBMonsterAnimInterface>(GetMesh()->GetAnimInstance());
		if (AnimInterface == nullptr)
		{
			//LOG_MSG(TEXT("AnimInterface casting failed"));
			return;
		}

		AnimInterface->SetAnimationType(EMonsterAnimType::BareHands);

		AttackMontage = LoadedMontage[EWeaponType::None].AttackMontage;
		DamagedMontage = LoadedMontage[EWeaponType::None].DamagedMontage;
		ParriedMontage = LoadedMontage[EWeaponType::None].ParriedMontage;
	}
	else if (MyWeaponType == EWeaponType::OneHand)
	{
		IBMonsterAnimInterface* AnimInterface = Cast<IBMonsterAnimInterface>(GetMesh()->GetAnimInstance());
		if (AnimInterface == nullptr)
		{
			//LOG_MSG(TEXT("AnimInterface casting failed"));
			return;
		}

		AnimInterface->SetAnimationType(EMonsterAnimType::OneHand);

		AttackMontage = LoadedMontage[EWeaponType::OneHand].AttackMontage;
		DamagedMontage = LoadedMontage[EWeaponType::OneHand].DamagedMontage;
		ParriedMontage = LoadedMontage[EWeaponType::OneHand].ParriedMontage;
	}
}

void ABMonsterFemale::CreateDamagedCollisionComponent()
{
	UCapsuleComponent* LUpperArm = CreateDefaultSubobject<UCapsuleComponent>(TEXT("LUpperArmCollision"));
	DamagedCollisions.Add(TEXT("LUpperArm"), { TEXT("LUpperArmCollisionSocket"), LUpperArm });

	UCapsuleComponent* RUpperArm = CreateDefaultSubobject<UCapsuleComponent>(TEXT("RUpperArmCollision"));
	DamagedCollisions.Add(TEXT("RUpperArm"), { TEXT("RUpperArmCollisionSocket"), RUpperArm });

	UCapsuleComponent* LForeArm = CreateDefaultSubobject<UCapsuleComponent>(TEXT("LForeArmCollision"));
	DamagedCollisions.Add(TEXT("LForeArm"), { TEXT("LForeArmCollisionSocket"), LForeArm });

	UCapsuleComponent* RForeArm = CreateDefaultSubobject<UCapsuleComponent>(TEXT("RForeArmCollision"));
	DamagedCollisions.Add(TEXT("RForeArm"), { TEXT("RForeArmCollisionSocket"), RForeArm });

	UCapsuleComponent* UpperBody = CreateDefaultSubobject<UCapsuleComponent>(TEXT("BodyCollision"));
	DamagedCollisions.Add(TEXT("UpperBody"), { TEXT("UpperBodyCollisionSocket"), UpperBody });

	UCapsuleComponent* HeadCol = CreateDefaultSubobject<UCapsuleComponent>(TEXT("HeadCollision"));
	DamagedCollisions.Add(TEXT("Head"), { TEXT("HeadCollisionSocket"), HeadCol });

	UCapsuleComponent* LThigh = CreateDefaultSubobject<UCapsuleComponent>(TEXT("LThighCollision"));
	DamagedCollisions.Add(TEXT("LThigh"), { TEXT("LThighCollisionSocket"), LThigh });

	UCapsuleComponent* RThigh = CreateDefaultSubobject<UCapsuleComponent>(TEXT("RThighCollision"));
	DamagedCollisions.Add(TEXT("RThigh"), { TEXT("RThighCollisionSocket"), RThigh });

	UCapsuleComponent* LShin = CreateDefaultSubobject<UCapsuleComponent>(TEXT("LShinCollision"));
	DamagedCollisions.Add(TEXT("LShin"), { TEXT("LShinCollisionSocket"), LShin });

	UCapsuleComponent* RShin = CreateDefaultSubobject<UCapsuleComponent>(TEXT("RShinCollision"));
	DamagedCollisions.Add(TEXT("RShin"), { TEXT("RShinCollisionSocket"), RShin });
}

void ABMonsterFemale::AttachDamagedCollisionComponentToMesh()
{
	UCapsuleComponent* LUpperArmColComp = DamagedCollisions[TEXT("LUpperArm")].Value;
	LUpperArmColComp->SetupAttachment(GetMesh(), FName(DamagedCollisions[TEXT("LUpperArm")].Key));
	LUpperArmColComp->SetCollisionProfileName(TEXT("MonsterCollision"));
	LUpperArmColComp->SetCapsuleSize(10.0f, 15.0f);
	//LUpperArmColComp->AttachToComponent(GetMesh(), FAttachmentTransformRules(EAttachmentRule::KeepRelative, true), FName(DamagedCollisions[TEXT("LUpperArm")].Key));

	UCapsuleComponent* RUpperArmColComp = DamagedCollisions[TEXT("RUpperArm")].Value;
	RUpperArmColComp->SetupAttachment(GetMesh(), FName(DamagedCollisions[TEXT("RUpperArm")].Key));
	RUpperArmColComp->SetCollisionProfileName(TEXT("MonsterCollision"));
	RUpperArmColComp->SetCapsuleSize(10.0f, 15.0f);
	//RUpperArmColComp->AttachToComponent(GetMesh(), FAttachmentTransformRules(EAttachmentRule::KeepRelative, true), FName(DamagedCollisions[TEXT("RUpperArm")].Key));

	UCapsuleComponent* LForeArmColComp = DamagedCollisions[TEXT("LForeArm")].Value;
	LForeArmColComp->SetupAttachment(GetMesh(), FName(DamagedCollisions[TEXT("LForeArm")].Key));
	LForeArmColComp->SetCollisionProfileName(TEXT("MonsterCollision"));
	LForeArmColComp->SetCapsuleSize(10.0f, 12.0f);
	//LForeArmColComp->AttachToComponent(GetMesh(), FAttachmentTransformRules(EAttachmentRule::KeepRelative, true), FName(DamagedCollisions[TEXT("LForeArm")].Key));

	UCapsuleComponent* RForeArmColComp = DamagedCollisions[TEXT("RForeArm")].Value;
	RForeArmColComp->SetupAttachment(GetMesh(), FName(DamagedCollisions[TEXT("RForeArm")].Key));
	RForeArmColComp->SetCollisionProfileName(TEXT("MonsterCollision"));
	RForeArmColComp->SetCapsuleSize(10.0f, 12.0f);
	//RForeArmColComp->AttachToComponent(GetMesh(), FAttachmentTransformRules(EAttachmentRule::KeepRelative, true), FName(DamagedCollisions[TEXT("RForeArm")].Key));

	UCapsuleComponent* UpperBodyColComp = DamagedCollisions[TEXT("UpperBody")].Value;
	UpperBodyColComp->SetupAttachment(GetMesh(), FName(DamagedCollisions[TEXT("UpperBody")].Key));
	UpperBodyColComp->SetCollisionProfileName(TEXT("MonsterCollision"));
	UpperBodyColComp->SetCapsuleSize(25.0f, 36.0f);
	//UpperBodyColComp->AttachToComponent(GetMesh(), FAttachmentTransformRules(EAttachmentRule::KeepRelative, true), FName(DamagedCollisions[TEXT("UpperBody")].Key));

	UCapsuleComponent* HeadColComp = DamagedCollisions[TEXT("Head")].Value;
	HeadColComp->SetupAttachment(GetMesh(), FName(DamagedCollisions[TEXT("Head")].Key));
	HeadColComp->SetCollisionProfileName(TEXT("MonsterCollision"));
	HeadColComp->SetCapsuleSize(15.0f, 18.0f);
	//HeadColComp->AttachToComponent(GetMesh(), FAttachmentTransformRules(EAttachmentRule::KeepRelative, true), FName(DamagedCollisions[TEXT("Head")].Key));

	UCapsuleComponent* LThighColComp = DamagedCollisions[TEXT("LThigh")].Value;
	LThighColComp->SetupAttachment(GetMesh(), FName(DamagedCollisions[TEXT("LThigh")].Key));
	LThighColComp->SetCollisionProfileName(TEXT("MonsterCollision"));
	LThighColComp->SetCapsuleSize(14.0f, 22.0f);
	//LThighColComp->AttachToComponent(GetMesh(), FAttachmentTransformRules(EAttachmentRule::KeepRelative, true), FName(DamagedCollisions[TEXT("LThigh")].Key));

	UCapsuleComponent* RThighColComp = DamagedCollisions[TEXT("RThigh")].Value;
	RThighColComp->SetupAttachment(GetMesh(), FName(DamagedCollisions[TEXT("RThigh")].Key));
	RThighColComp->SetCollisionProfileName(TEXT("MonsterCollision"));
	RThighColComp->SetCapsuleSize(14.0f, 22.0f);
	//RThighColComp->AttachToComponent(GetMesh(), FAttachmentTransformRules(EAttachmentRule::KeepRelative, true), FName(DamagedCollisions[TEXT("RThigh")].Key));

	UCapsuleComponent* LShinColComp = DamagedCollisions[TEXT("LShin")].Value;
	LShinColComp->SetupAttachment(GetMesh(), FName(DamagedCollisions[TEXT("LShin")].Key));
	LShinColComp->SetCollisionProfileName(TEXT("MonsterCollision"));
	LShinColComp->SetCapsuleSize(14.0f, 22.0f);
	//LShinColComp->AttachToComponent(GetMesh(), FAttachmentTransformRules(EAttachmentRule::KeepRelative, true), FName(DamagedCollisions[TEXT("LShin")].Key));

	UCapsuleComponent* RShinColComp = DamagedCollisions[TEXT("RShin")].Value;
	RShinColComp->SetupAttachment(GetMesh(), FName(DamagedCollisions[TEXT("RShin")].Key));
	RShinColComp->SetCollisionProfileName(TEXT("MonsterCollision"));
	RShinColComp->SetCapsuleSize(14.0f, 22.0f);
	//RShinColComp->AttachToComponent(GetMesh(), FAttachmentTransformRules(EAttachmentRule::KeepRelative, true), FName(DamagedCollisions[TEXT("RShin")].Key));

	//for (TPair<FString, TPair<FString, TObjectPtr<class UCapsuleComponent>>> Pair : DamagedCollisions)
	//{
	//	Pair.Value.Value->SetHiddenInGame(false);
	//}
}