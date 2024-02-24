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
	CreateDamagedCollisionComponent();

	MeshLoad();
	AnimInstanceLoad();
	MontageLoad();

	InitValue();
	InitAI();
	InitSoundCues();

	AttachDamagedCollisionComponentToMesh();

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
	if (LoadedAnimInstance[EMeshAnimType::Base] == nullptr)
	{
		//LOG_MSG(TEXT("Anim Instance is NullPtr"));
	}

	if (LoadedAnimInstance[EMeshAnimType::Copy] == nullptr)
	{
		//LOG_MSG(TEXT("Copy Anim Instance is NullPtr"));
	}

	GetMesh()->SetAnimInstanceClass(LoadedAnimInstance[EMeshAnimType::Base]);

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

void ABMonsterMale::AllCollisionOff()
{
	if (GetCapsuleComponent() != nullptr)
	{
		GetCapsuleComponent()->SetCollisionProfileName(TEXT("NoCollision"));
	}

	if (GetMesh() != nullptr)
	{
		GetMesh()->SetCollisionProfileName(TEXT("NoCollision"));
	}

	if (Jacket != nullptr)
	{
		Jacket->SetCollisionProfileName(TEXT("NoCollision"));
	}

	if (Hat != nullptr)
	{
		Hat->SetCollisionProfileName(TEXT("NoCollision"));
	}

	if (BodyBase != nullptr)
	{
		BodyBase->SetCollisionProfileName(TEXT("NoCollision"));
	}

	if (Pants != nullptr)
	{
		Pants->SetCollisionProfileName(TEXT("NoCollision"));
	}

	if (Weapon != nullptr)
	{
		Weapon->SetCollisionProfileName(TEXT("NoCollision"));
	}

	if (Head != nullptr)
	{
		Head->SetCollisionProfileName(TEXT("NoCollision"));
	}
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
		GeneralSoundCues.Add(ESoundType::Yell, YellSoundCue);
	}
	
	static ConstructorHelpers::FObjectFinder<USoundCue> IdleSoundRef(TEXT("/Script/Engine.SoundCue'/Game/Blueprints/Actor/Monster/SoundCue/SC_IdleSoundCue.SC_IdleSoundCue'"));
	if (IdleSoundRef.Object != nullptr)
	{
		USoundCue* IdleSoundCue = IdleSoundRef.Object;
		GeneralSoundCues.Add(ESoundType::Idle, IdleSoundCue);
	}
	
	static ConstructorHelpers::FObjectFinder<USoundCue> DetectSoundRef(TEXT("/Script/Engine.SoundCue'/Game/Blueprints/Actor/Monster/SoundCue/SC_DetectPlayer.SC_DetectPlayer'"));
	if (DetectSoundRef.Object != nullptr)
	{
		USoundCue* DetectSoundCue = DetectSoundRef.Object;
		GeneralSoundCues.Add(ESoundType::Detect, DetectSoundCue);
	}
	
	static ConstructorHelpers::FObjectFinder<USoundCue> DamagedSoundRef(TEXT("/Script/Engine.SoundCue'/Game/Blueprints/Actor/Monster/SoundCue/SC_DamagedCue.SC_DamagedCue'"));
	if (DamagedSoundRef.Object != nullptr)
	{
		USoundCue* DamagedSoundCue = DamagedSoundRef.Object;
		GeneralSoundCues.Add(ESoundType::Damaged, DamagedSoundCue);
	}
	
	static ConstructorHelpers::FObjectFinder<USoundCue> AttackSoundRef(TEXT("/Script/Engine.SoundCue'/Game/Blueprints/Actor/Monster/SoundCue/SC_AttackCue.SC_AttackCue'"));
	if (AttackSoundRef.Object != nullptr)
	{
		USoundCue* AttackCue = AttackSoundRef.Object;
		GeneralSoundCues.Add(ESoundType::Attack, AttackCue);
	}

	static ConstructorHelpers::FObjectFinder<USoundCue> GeneralGroggySoundRef(TEXT("/Script/Engine.SoundCue'/Game/Blueprints/Actor/Monster/SoundCue/SC_GenaralGroggyCue.SC_GenaralGroggyCue'"));
	if (GeneralGroggySoundRef.Object != nullptr)
	{
		USoundCue* GenaralGroggyCue = GeneralGroggySoundRef.Object;
		GeneralSoundCues.Add(ESoundType::GeneralGroggy, GenaralGroggyCue);
	}
	
	static ConstructorHelpers::FObjectFinder<USoundCue> HeadShotGroggySoundRef(TEXT("/Script/Engine.SoundCue'/Game/Blueprints/Actor/Monster/SoundCue/SC_HeadShotGroggyCue.SC_HeadShotGroggyCue'"));
	if (HeadShotGroggySoundRef.Object != nullptr)
	{
		USoundCue* HeadShotGroggyCue = HeadShotGroggySoundRef.Object;
		GeneralSoundCues.Add(ESoundType::HeadShotGroggy, HeadShotGroggyCue);
	}
	
	static ConstructorHelpers::FObjectFinder<USoundCue> PointDeathSoundRef(TEXT("/Script/Engine.SoundCue'/Game/Blueprints/Actor/Monster/SoundCue/SC_PointDeathCue.SC_PointDeathCue'"));
	if (PointDeathSoundRef.Object != nullptr)
	{
		USoundCue* PointDeathGroggyCue = PointDeathSoundRef.Object;
		GeneralSoundCues.Add(ESoundType::PointDeath, PointDeathGroggyCue);
	}

	static ConstructorHelpers::FObjectFinder<USoundCue> ParrySoundRef(TEXT("/Script/Engine.SoundCue'/Game/Blueprints/Actor/Monster/SoundCue/SC_ParryCue.SC_ParryCue'"));
	if (ParrySoundRef.Object != nullptr)
	{
		USoundCue* ParryCue = ParrySoundRef.Object;
		GeneralSoundCues.Add(ESoundType::Parry, ParryCue);
	}

	static ConstructorHelpers::FObjectFinder<USoundCue> KickedSoundRef(TEXT("/Script/Engine.SoundCue'/Game/Blueprints/Actor/Monster/SoundCue/SC_KickedCue.SC_KickedCue'"));
	if (KickedSoundRef.Object != nullptr)
	{
		USoundCue* KickedCue = KickedSoundRef.Object;
		GeneralSoundCues.Add(ESoundType::Kicked, KickedCue);
	}

	static ConstructorHelpers::FObjectFinder<USoundCue> MutterSoundRef(TEXT("/Script/Engine.SoundCue'/Game/Blueprints/Actor/Monster/SoundCue/SC_MutterCue.SC_MutterCue'"));
	if (MutterSoundRef.Object != nullptr)
	{
		USoundCue* MutterCue = MutterSoundRef.Object;
		GeneralSoundCues.Add(ESoundType::Mutter, MutterCue);
	}
}

void ABMonsterMale::MeshLoad()
{
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> BodyRef(TEXT("/Script/Engine.SkeletalMesh'/Game/Assets/Monster/Mesh/BasicMonster/Male/Base/SK_MonsterMale.SK_MonsterMale'"));
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

	if (0 == LoadedMesh.Num())
	{
		LoadedMesh.Add(EMeshType::Hat, TArray<TObjectPtr<class USkeletalMesh>>());
		LoadedMesh.Add(EMeshType::Head, TArray<TObjectPtr<class USkeletalMesh>>());
		LoadedMesh.Add(EMeshType::Jacket, TArray<TObjectPtr<class USkeletalMesh>>());
		LoadedMesh.Add(EMeshType::Pants, TArray<TObjectPtr<class USkeletalMesh>>());

		LoadedMesh[EMeshType::Head].Add(Head1Ref.Object);
		LoadedMesh[EMeshType::Head].Add(Head2Ref.Object);
		LoadedMesh[EMeshType::Head].Add(Head3Ref.Object);
		LoadedMesh[EMeshType::Head].Add(Head4Ref.Object);

		LoadedMesh[EMeshType::Hat].Add(Hat1Ref.Object);
		LoadedMesh[EMeshType::Hat].Add(Hat2Ref.Object);
		LoadedMesh[EMeshType::Hat].Add(Hat3Ref.Object);

		LoadedMesh[EMeshType::Jacket].Add(Jacket1Ref.Object);
		LoadedMesh[EMeshType::Jacket].Add(Jacket2Ref.Object);
		LoadedMesh[EMeshType::Jacket].Add(Jacket3Ref.Object);
		LoadedMesh[EMeshType::Jacket].Add(Jacket4Ref.Object);

		LoadedMesh[EMeshType::Pants].Add(Pants1Ref.Object);
		LoadedMesh[EMeshType::Pants].Add(Pants2Ref.Object);
		LoadedMesh[EMeshType::Pants].Add(Pants3Ref.Object);
	}
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
	static ConstructorHelpers::FClassFinder<UAnimInstance> BaseAnimRef(TEXT("/Game/Blueprints/Actor/Monster/ABP_BasicMonsterMale.ABP_BasicMonsterMale_C"));
	static ConstructorHelpers::FClassFinder<UAnimInstance> CopyAnimRef(TEXT("/Game/Blueprints/Actor/Monster/ABP_BasicMonsterMaleCopy.ABP_BasicMonsterMaleCopy_C"));
	
	LoadedAnimInstance.Add(EMeshAnimType::Base, BaseAnimRef.Class);
	LoadedAnimInstance.Add(EMeshAnimType::Copy, CopyAnimRef.Class);
}

void ABMonsterMale::Tick(float _DeltaTIme)
{
	Super::Tick(_DeltaTIme);

	IBMonsterAnimInterface* AnimInterface = Cast<IBMonsterAnimInterface>(GetMesh()->GetAnimInstance());
	if (AnimInterface == nullptr)
	{
		//LOG_MSG(TEXT("AnimInterface casting failed"));
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

void ABMonsterMale::CreateDamagedCollisionComponent()
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

void ABMonsterMale::AttachDamagedCollisionComponentToMesh()
{
	UCapsuleComponent* LUpperArmColComp = DamagedCollisions[TEXT("LUpperArm")].Value;
	LUpperArmColComp->SetupAttachment(GetMesh(), FName(DamagedCollisions[TEXT("LUpperArm")].Key));
	LUpperArmColComp->SetCollisionProfileName(TEXT("MonsterCollision"));
	LUpperArmColComp->SetCapsuleSize(10.0f, 15.0f);
	//LUpperArmColComp->AttachToComponent(GetMesh(), FAttachmentTransformRules(EAttachmentRule::KeepRelative, true), FName(DamagedCollisions[TEXT("LUpperArm")].Key));
	
	UCapsuleComponent* RUpperArmColComp = DamagedCollisions[TEXT("RUpperArm")].Value;
	RUpperArmColComp->SetupAttachment(GetMesh(),FName(DamagedCollisions[TEXT("RUpperArm")].Key));
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
