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
	CreateSkeletalMeshComponent();

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
	AutoPossessAI = EAutoPossessAI::Disabled;

	GetCharacterMovement()->MaxWalkSpeed = 100.0f;
	GetCharacterMovement()->bUseControllerDesiredRotation = true;

	CreateDamagedCollisionComponent();
	AttachDamagedCollisionComponentToMesh();

	InitDamageTypes();
}

void ABChainsawMan::AIStart()
{
	ABAIChainsawManController* MyController = GetWorld()->SpawnActor<ABAIChainsawManController>(AIControllerClass, FVector::ZeroVector, FRotator::ZeroRotator);
	
	if (MyController)
	{
		MyController->Possess(this);
	}
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
		//LOG_WARNING(TEXT("AIController is nullptr"));
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

void ABChainsawMan::AllCollisionOff()
{
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("NoCollision"));
	GetMesh()->SetCollisionProfileName(TEXT("NoCollision"));
}

void ABChainsawMan::CreateDamagedCollisionComponent()
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

void ABChainsawMan::CreateSkeletalMeshComponent()
{
	HeadMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Head"));
	ShirtMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Shirt"));
	PantsMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Pants"));
	
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> ArmMeshRef(TEXT("/Script/Engine.SkeletalMesh'/Game/Assets/Monster/Mesh/ChainsawMan/Arm/SK_ChainsawManArm.SK_ChainsawManArm'"));
	if (ArmMeshRef.Object != nullptr)
	{
		GetMesh()->SetSkeletalMesh(ArmMeshRef.Object);
	}

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> HeadMeshRef(TEXT("/Script/Engine.SkeletalMesh'/Game/Assets/Monster/Mesh/ChainsawMan/Head/SK_ChainsawManHead.SK_ChainsawManHead'"));
	if (HeadMeshRef.Object != nullptr)
	{
		HeadMesh->SetSkeletalMesh(HeadMeshRef.Object);
	}

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> PantsMeshRef(TEXT("/Script/Engine.SkeletalMesh'/Game/Assets/Monster/Mesh/ChainsawMan/Pants/SK_ChainsawManPants.SK_ChainsawManPants'"));
	if (PantsMeshRef.Object != nullptr)
	{
		PantsMesh->SetSkeletalMesh(PantsMeshRef.Object);
	}

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> ShirtMeshRef(TEXT("/Script/Engine.SkeletalMesh'/Game/Assets/Monster/Mesh/ChainsawMan/Shirt/SK_ChainsawManShirt.SK_ChainsawManShirt'"));
	if (ShirtMeshRef.Object != nullptr)
	{
		ShirtMesh->SetSkeletalMesh(ShirtMeshRef.Object);
	}

	HeadMesh->SetupAttachment(RootComponent);
	ShirtMesh->SetupAttachment(RootComponent);
	PantsMesh->SetupAttachment(RootComponent);
}

void ABChainsawMan::AttachDamagedCollisionComponentToMesh()
{
	UCapsuleComponent* LUpperArmColComp = DamagedCollisions[TEXT("LUpperArm")].Value;
	LUpperArmColComp->SetupAttachment(ShirtMesh, FName(DamagedCollisions[TEXT("LUpperArm")].Key));
	LUpperArmColComp->SetCollisionProfileName(TEXT("MonsterCollision"));
	LUpperArmColComp->SetCapsuleSize(10.0f, 15.0f);
	//LUpperArmColComp->AttachToComponent(ShirtMesh, FAttachmentTransformRules(EAttachmentRule::KeepRelative, true), FName(DamagedCollisions[TEXT("LUpperArm")].Key));

	UCapsuleComponent* RUpperArmColComp = DamagedCollisions[TEXT("RUpperArm")].Value;
	RUpperArmColComp->SetupAttachment(ShirtMesh, FName(DamagedCollisions[TEXT("RUpperArm")].Key));
	RUpperArmColComp->SetCollisionProfileName(TEXT("MonsterCollision"));
	RUpperArmColComp->SetCapsuleSize(10.0f, 15.0f);
	//RUpperArmColComp->AttachToComponent(ShirtMesh, FAttachmentTransformRules(EAttachmentRule::KeepRelative, true), FName(DamagedCollisions[TEXT("RUpperArm")].Key));

	UCapsuleComponent* LForeArmColComp = DamagedCollisions[TEXT("LForeArm")].Value;
	LForeArmColComp->SetupAttachment(ShirtMesh, FName(DamagedCollisions[TEXT("LForeArm")].Key));
	LForeArmColComp->SetCollisionProfileName(TEXT("MonsterCollision"));
	LForeArmColComp->SetCapsuleSize(10.0f, 12.0f);
	//LForeArmColComp->AttachToComponent(ShirtMesh, FAttachmentTransformRules(EAttachmentRule::KeepRelative, true), FName(DamagedCollisions[TEXT("LForeArm")].Key));

	UCapsuleComponent* RForeArmColComp = DamagedCollisions[TEXT("RForeArm")].Value;
	RForeArmColComp->SetupAttachment(ShirtMesh, FName(DamagedCollisions[TEXT("RForeArm")].Key));
	RForeArmColComp->SetCollisionProfileName(TEXT("MonsterCollision"));
	RForeArmColComp->SetCapsuleSize(10.0f, 12.0f);
	//RForeArmColComp->AttachToComponent(ShirtMesh, FAttachmentTransformRules(EAttachmentRule::KeepRelative, true), FName(DamagedCollisions[TEXT("RForeArm")].Key));

	UCapsuleComponent* UpperBodyColComp = DamagedCollisions[TEXT("UpperBody")].Value;
	UpperBodyColComp->SetupAttachment(ShirtMesh, FName(DamagedCollisions[TEXT("UpperBody")].Key));
	UpperBodyColComp->SetCollisionProfileName(TEXT("MonsterCollision"));
	UpperBodyColComp->SetCapsuleSize(25.0f, 36.0f);
	//UpperBodyColComp->AttachToComponent(ShirtMesh, FAttachmentTransformRules(EAttachmentRule::KeepRelative, true), FName(DamagedCollisions[TEXT("UpperBody")].Key));
	
	UCapsuleComponent* HeadColComp = DamagedCollisions[TEXT("Head")].Value;
	HeadColComp->SetupAttachment(HeadMesh, FName(DamagedCollisions[TEXT("Head")].Key));
	HeadColComp->SetCollisionProfileName(TEXT("MonsterCollision"));
	HeadColComp->SetCapsuleSize(15.0f, 18.0f);
	//HeadColComp->AttachToComponent(HeadMesh, FAttachmentTransformRules(EAttachmentRule::KeepRelative, true), FName(DamagedCollisions[TEXT("Head")].Key));

	UCapsuleComponent* LThighColComp = DamagedCollisions[TEXT("LThigh")].Value;
	LThighColComp->SetupAttachment(PantsMesh, FName(DamagedCollisions[TEXT("LThigh")].Key));
	LThighColComp->SetCollisionProfileName(TEXT("MonsterCollision"));
	LThighColComp->SetCapsuleSize(14.0f, 22.0f);
	//LThighColComp->AttachToComponent(PantsMesh, FAttachmentTransformRules(EAttachmentRule::KeepRelative, true), FName(DamagedCollisions[TEXT("LThigh")].Key));

	UCapsuleComponent* RThighColComp = DamagedCollisions[TEXT("RThigh")].Value;
	RThighColComp->SetupAttachment(PantsMesh, FName(DamagedCollisions[TEXT("RThigh")].Key));
	RThighColComp->SetCollisionProfileName(TEXT("MonsterCollision"));
	RThighColComp->SetCapsuleSize(14.0f, 22.0f);
	//RThighColComp->AttachToComponent(PantsMesh, FAttachmentTransformRules(EAttachmentRule::KeepRelative, true), FName(DamagedCollisions[TEXT("RThigh")].Key));

	UCapsuleComponent* LShinColComp = DamagedCollisions[TEXT("LShin")].Value;
	LShinColComp->SetupAttachment(PantsMesh, FName(DamagedCollisions[TEXT("LShin")].Key));
	LShinColComp->SetCollisionProfileName(TEXT("MonsterCollision"));
	LShinColComp->SetCapsuleSize(14.0f, 22.0f);
	//LShinColComp->AttachToComponent(PantsMesh, FAttachmentTransformRules(EAttachmentRule::KeepRelative, true), FName(DamagedCollisions[TEXT("LShin")].Key));

	UCapsuleComponent* RShinColComp = DamagedCollisions[TEXT("RShin")].Value;
	RShinColComp->SetupAttachment(PantsMesh, FName(DamagedCollisions[TEXT("RShin")].Key));
	RShinColComp->SetCollisionProfileName(TEXT("MonsterCollision"));
	RShinColComp->SetCapsuleSize(14.0f, 22.0f);
	//RShinColComp->AttachToComponent(PantsMesh, FAttachmentTransformRules(EAttachmentRule::KeepRelative, true), FName(DamagedCollisions[TEXT("RShin")].Key));
}


