// Fill out your copyright notice in the Description page of Project Settings.


#include "BMonsterMale.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"

#include "BiohazardRe4.h"
#include "Sound/SoundCue.h"

#include "Actor/Monster/Component/BMonsterStatComponent.h"
#include "Actor/Monster/Interface/BMonsterAnimInterface.h"

TMap<EMeshType, TArray<TObjectPtr<USkeletalMesh>>> ABMonsterMale::LoadedMesh;
TMap<EMeshAnimType, TSubclassOf<UAnimInstance>> ABMonsterMale::LoadedAnimInstance;
TMap<EWeaponType, FMontageStruct> ABMonsterMale::LoadedMontage;

ABMonsterMale::ABMonsterMale()
{
	CreateComponent();
	MeshLoad();
	AnimInstanceLoad();
	MontageLoad();

	InitValue();
	InitAI();
	InitSoundCues();

	PrimaryActorTick.bCanEverTick = true;
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
	GetMesh()->SetAnimInstanceClass(LoadedAnimInstance[EMeshAnimType::Base]);

	Head->SetAnimInstanceClass(LoadedAnimInstance[EMeshAnimType::Copy]);
	Hat->SetAnimInstanceClass(LoadedAnimInstance[EMeshAnimType::Copy]);
	Jacket->SetAnimInstanceClass(LoadedAnimInstance[EMeshAnimType::Copy]);
	Pants->SetAnimInstanceClass(LoadedAnimInstance[EMeshAnimType::Copy]);

	if (MyWeaponType == EWeaponType::None)
	{
		IBMonsterAnimInterface* AnimInterface = Cast<IBMonsterAnimInterface>(GetMesh()->GetAnimInstance());
		if (AnimInterface == nullptr)
		{
			LOG_MSG(TEXT("AnimInterface casting failed"));
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
			LOG_MSG(TEXT("AnimInterface casting failed"));
			return;
		}
		
		AnimInterface->SetAnimationType(EMonsterAnimType::OneHand);
		
		AttackMontage = LoadedMontage[EWeaponType::OneHand].AttackMontage;
		DamagedMontage = LoadedMontage[EWeaponType::OneHand].DamagedMontage;
		ParriedMontage = LoadedMontage[EWeaponType::OneHand].ParriedMontage;
	}
}

void ABMonsterMale::AllCollisionOff()
{
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("NoCollision"));
	GetMesh()->SetCollisionProfileName(TEXT("NoCollision"));

	Jacket->SetCollisionProfileName(TEXT("NoCollision"));
	Hat->SetCollisionProfileName(TEXT("NoCollision"));
	BodyBase->SetCollisionProfileName(TEXT("NoCollision"));
	Pants->SetCollisionProfileName(TEXT("NoCollision"));
	Weapon->SetCollisionProfileName(TEXT("NoCollision"));
	Head->SetCollisionProfileName(TEXT("NoCollision"));
}

void ABMonsterMale::SetSkeletalMeshInConstructor()
{

}

void ABMonsterMale::InitSoundCues()
{
	static ConstructorHelpers::FObjectFinder<USoundCue> YellSoundRef(TEXT("/Script/Engine.SoundCue'/Game/Blueprints/Actor/Monster/SoundCue/SC_ChaseSoundCue.SC_ChaseSoundCue'"));
	if (YellSoundRef.Object != nullptr)
	{
		USoundCue* YellSoundCue = YellSoundRef.Object;
		SoundCues.Add(ESoundType::Yell, YellSoundCue);
	}
	
	static ConstructorHelpers::FObjectFinder<USoundCue> IdleSoundRef(TEXT("/Script/Engine.SoundCue'/Game/Blueprints/Actor/Monster/SoundCue/SC_IdleSoundCue.SC_IdleSoundCue'"));
	if (IdleSoundRef.Object != nullptr)
	{
		USoundCue* IdleSoundCue = IdleSoundRef.Object;
		SoundCues.Add(ESoundType::Idle, IdleSoundCue);
	}
	
	static ConstructorHelpers::FObjectFinder<USoundCue> DetectSoundRef(TEXT("/Script/Engine.SoundCue'/Game/Blueprints/Actor/Monster/SoundCue/SC_DetectPlayer.SC_DetectPlayer'"));
	if (DetectSoundRef.Object != nullptr)
	{
		USoundCue* DetectSoundCue = DetectSoundRef.Object;
		SoundCues.Add(ESoundType::Detect, DetectSoundCue);
	}
	
	static ConstructorHelpers::FObjectFinder<USoundCue> DamagedSoundRef(TEXT("/Script/Engine.SoundCue'/Game/Blueprints/Actor/Monster/SoundCue/SC_DamagedCue.SC_DamagedCue'"));
	if (DamagedSoundRef.Object != nullptr)
	{
		USoundCue* DamagedSoundCue = DamagedSoundRef.Object;
		SoundCues.Add(ESoundType::Damaged, DamagedSoundCue);
	}
	
	static ConstructorHelpers::FObjectFinder<USoundCue> AttackSoundRef(TEXT("/Script/Engine.SoundCue'/Game/Blueprints/Actor/Monster/SoundCue/SC_AttackCue.SC_AttackCue'"));
	if (AttackSoundRef.Object != nullptr)
	{
		USoundCue* AttackCue = AttackSoundRef.Object;
		SoundCues.Add(ESoundType::Attack, AttackCue);
	}

	static ConstructorHelpers::FObjectFinder<USoundCue> GeneralGroggySoundRef(TEXT("/Script/Engine.SoundCue'/Game/Blueprints/Actor/Monster/SoundCue/SC_GenaralGroggyCue.SC_GenaralGroggyCue'"));
	if (GeneralGroggySoundRef.Object != nullptr)
	{
		USoundCue* GenaralGroggyCue = GeneralGroggySoundRef.Object;
		SoundCues.Add(ESoundType::GeneralGroggy, GenaralGroggyCue);
	}
	
	static ConstructorHelpers::FObjectFinder<USoundCue> HeadShotGroggySoundRef(TEXT("/Script/Engine.SoundCue'/Game/Blueprints/Actor/Monster/SoundCue/SC_HeadShotGroggyCue.SC_HeadShotGroggyCue'"));
	if (HeadShotGroggySoundRef.Object != nullptr)
	{
		USoundCue* HeadShotGroggyCue = HeadShotGroggySoundRef.Object;
		SoundCues.Add(ESoundType::HeadShotGroggy, HeadShotGroggyCue);
	}
	
	static ConstructorHelpers::FObjectFinder<USoundCue> PointDeathSoundRef(TEXT("/Script/Engine.SoundCue'/Game/Blueprints/Actor/Monster/SoundCue/SC_PointDeathCue.SC_PointDeathCue'"));
	if (PointDeathSoundRef.Object != nullptr)
	{
		USoundCue* PointDeathGroggyCue = PointDeathSoundRef.Object;
		SoundCues.Add(ESoundType::PointDeath, PointDeathGroggyCue);
	}

}

void ABMonsterMale::MeshLoad()
{
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> BodyRef(TEXT("/Script/Engine.SkeletalMesh'/Game/Assets/Monster/Mesh/BasicMonster/Male/Base/SK_MonsterMaleBase.SK_MonsterMaleBase'"));
	if (BodyRef.Object != nullptr)
	{
		GetMesh()->SetSkeletalMesh(BodyRef.Object);
		BodyBase = GetMesh();
	}

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> Head1Ref(TEXT("/Script/Engine.SkeletalMesh'/Game/Assets/Monster/Mesh/BasicMonster/Male/Head/1/SK_MonsterMaleHead_1.SK_MonsterMaleHead_1'"));
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> Head2Ref(TEXT("/Script/Engine.SkeletalMesh'/Game/Assets/Monster/Mesh/BasicMonster/Male/Head/2/SK_MonsterMaleHead_2.SK_MonsterMaleHead_2'"));
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> Head3Ref(TEXT("/Script/Engine.SkeletalMesh'/Game/Assets/Monster/Mesh/BasicMonster/Male/Head/3/SK_MonsterMaleHead_3.SK_MonsterMaleHead_3'"));
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> Head4Ref(TEXT("/Script/Engine.SkeletalMesh'/Game/Assets/Monster/Mesh/BasicMonster/Male/Head/4/SK_MonsterMaleHead_4.SK_MonsterMaleHead_4'"));
	
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> Hat1Ref(TEXT("/Script/Engine.SkeletalMesh'/Game/Assets/Monster/Mesh/BasicMonster/Male/Hat/1/SK_MonsterMaleHat_1.SK_MonsterMaleHat_1'"));
	static ConstructorHelpers::FObjectFinder<USkeletalMesh>	Hat2Ref(TEXT("/Script/Engine.SkeletalMesh'/Game/Assets/Monster/Mesh/BasicMonster/Male/Hat/2/SK_MonsterMaleHat_2.SK_MonsterMaleHat_2'"));
	static ConstructorHelpers::FObjectFinder<USkeletalMesh>	Hat3Ref(TEXT("/Script/Engine.SkeletalMesh'/Game/Assets/Monster/Mesh/BasicMonster/Male/Hat/3/SK_MonsterMaleHat_3.SK_MonsterMaleHat_3'"));

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> Jacket1Ref(TEXT("/Script/Engine.SkeletalMesh'/Game/Assets/Monster/Mesh/BasicMonster/Male/Jacket/1/SK_MonsterMaleJacket_1.SK_MonsterMaleJacket_1'"));
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> Jacket2Ref(TEXT("/Script/Engine.SkeletalMesh'/Game/Assets/Monster/Mesh/BasicMonster/Male/Jacket/2/SK_MonsterMaleJacket_2.SK_MonsterMaleJacket_2'"));
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> Jacket3Ref(TEXT("/Script/Engine.SkeletalMesh'/Game/Assets/Monster/Mesh/BasicMonster/Male/Jacket/3/SK_MonsterMaleJacket_3.SK_MonsterMaleJacket_3'"));
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> Jacket4Ref(TEXT("/Script/Engine.SkeletalMesh'/Game/Assets/Monster/Mesh/BasicMonster/Male/Jacket/4/SK_MonsterMaleJacket_4.SK_MonsterMaleJacket_4'"));

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> Pants1Ref(TEXT("/Script/Engine.SkeletalMesh'/Game/Assets/Monster/Mesh/BasicMonster/Male/Pants/1/SK_MonsterMalePants_1.SK_MonsterMalePants_1'"));
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> Pants2Ref(TEXT("/Script/Engine.SkeletalMesh'/Game/Assets/Monster/Mesh/BasicMonster/Male/Pants/2/SK_MonsterMalePants_2.SK_MonsterMalePants_2'"));
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> Pants3Ref(TEXT("/Script/Engine.SkeletalMesh'/Game/Assets/Monster/Mesh/BasicMonster/Male/Pants/3/SK_MonsterMalePants_3.SK_MonsterMalePants_3'"));

	LoadedMesh.Add(EMeshType::Hat, TArray<TObjectPtr<class USkeletalMesh>>());
	LoadedMesh.Add(EMeshType::Head, TArray<TObjectPtr<class USkeletalMesh>>());
	LoadedMesh.Add(EMeshType::Jacket, TArray<TObjectPtr<class USkeletalMesh>>());
	LoadedMesh.Add(EMeshType::Pants, TArray<TObjectPtr<class USkeletalMesh>>());

	LoadedMesh[EMeshType::Hat].Add(Head1Ref.Object);
	LoadedMesh[EMeshType::Hat].Add(Head2Ref.Object);
	LoadedMesh[EMeshType::Hat].Add(Head3Ref.Object);
	LoadedMesh[EMeshType::Hat].Add(Head4Ref.Object);

	LoadedMesh[EMeshType::Head].Add(Hat1Ref.Object);
	LoadedMesh[EMeshType::Head].Add(Hat2Ref.Object);
	LoadedMesh[EMeshType::Head].Add(Hat3Ref.Object);

	LoadedMesh[EMeshType::Jacket].Add(Jacket1Ref.Object);
	LoadedMesh[EMeshType::Jacket].Add(Jacket2Ref.Object);
	LoadedMesh[EMeshType::Jacket].Add(Jacket3Ref.Object);
	LoadedMesh[EMeshType::Jacket].Add(Jacket4Ref.Object);

	LoadedMesh[EMeshType::Pants].Add(Pants1Ref.Object);
	LoadedMesh[EMeshType::Pants].Add(Pants2Ref.Object);
	LoadedMesh[EMeshType::Pants].Add(Pants3Ref.Object);
}

void ABMonsterMale::MontageLoad()
{
		static ConstructorHelpers::FObjectFinder<UAnimMontage> None_AttackMontageRef(TEXT("/Script/Engine.AnimMontage'/Game/Blueprints/Actor/Monster/Animation/AM_MonsterMale_Attack_BareHands.AM_MonsterMale_Attack_BareHands'"));
		static ConstructorHelpers::FObjectFinder<UAnimMontage> None_DamagedMontageRef(TEXT("/Script/Engine.AnimMontage'/Game/Blueprints/Actor/Monster/Animation/AM_MonsterMale_Damaged_BareHands.AM_MonsterMale_Damaged_BareHands'"));
		static ConstructorHelpers::FObjectFinder<UAnimMontage> None_ParriedMontageRef(TEXT("/Script/Engine.AnimMontage'/Game/Blueprints/Actor/Monster/Animation/AM_MonsterMale_Parry_Bare_Hands.AM_MonsterMale_Parry_Bare_Hands'"));
	
		static ConstructorHelpers::FObjectFinder<UAnimMontage> One_AttackMontageRef(TEXT("/Script/Engine.AnimMontage'/Game/Blueprints/Actor/Monster/Animation/AM_MonsterMale_Attack_OneHand.AM_MonsterMale_Attack_OneHand'"));
		static ConstructorHelpers::FObjectFinder<UAnimMontage> One_DamagedMontageRef(TEXT("/Script/Engine.AnimMontage'/Game/Blueprints/Actor/Monster/Animation/AM_MonsterMale_Damaged_OneHand.AM_MonsterMale_Damaged_OneHand'"));
		static ConstructorHelpers::FObjectFinder<UAnimMontage> One_ParriedMontageRef(TEXT("/Script/Engine.AnimMontage'/Game/Blueprints/Actor/Monster/Animation/AM_MonsterMale_Parry_One_Hand.AM_MonsterMale_Parry_One_Hand'"));

		LoadedMontage.Add(EWeaponType::None, { None_AttackMontageRef.Object, None_DamagedMontageRef.Object, None_ParriedMontageRef.Object });
		LoadedMontage.Add(EWeaponType::OneHand, { One_AttackMontageRef.Object, One_DamagedMontageRef.Object, One_ParriedMontageRef.Object });
}

void ABMonsterMale::AnimInstanceLoad()
{
	static ConstructorHelpers::FClassFinder<UAnimInstance> BaseAnimRef(TEXT("/Game/Blueprints/Actor/Monster/ABP_BasicMonsterMale.ABP_BasicMonsterMale"));
	static ConstructorHelpers::FClassFinder<UAnimInstance> CopyAnimRef(TEXT("/Game/Blueprints/Actor/Monster/ABP_BasicMonsterMaleCopy.ABP_BasicMonsterMaleCopy"));

	LoadedAnimInstance.Add(EMeshAnimType::Base, BaseAnimRef.Class);
	LoadedAnimInstance.Add(EMeshAnimType::Copy, CopyAnimRef.Class);
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
	//¸Ó¸®, ¸ðÀÚ ½Ö
	int HeadHatIndex = FMath::RandRange(0, 3);

	if (LoadedMesh[EMeshType::Head][HeadHatIndex] == nullptr)
	{
		LOG_FATAL(TEXT("Head Mesh is failed to load"));
		return;
	}
	else
	{
		Head->SetSkeletalMesh(LoadedMesh[EMeshType::Head][HeadHatIndex]);
	}

	if (HeadHatIndex == 3)
	{
		Hat->SetVisibility(false);
	}
	else
	{
		if (LoadedMesh[EMeshType::Hat][HeadHatIndex] == nullptr)
		{
			LOG_FATAL(TEXT("Hat Mesh is failed to load"));
			return;
		}
		else
		{
			Hat->SetSkeletalMesh(LoadedMesh[EMeshType::Hat][HeadHatIndex]);
		}
	}

	//Jacket
	int JacketIndex = FMath::RandRange(0, 3);

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
	int PantsIndex = FMath::RandRange(0, 2);

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

