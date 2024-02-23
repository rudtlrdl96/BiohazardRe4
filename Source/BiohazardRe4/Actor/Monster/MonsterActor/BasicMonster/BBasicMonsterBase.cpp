

#include "BBasicMonsterBase.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BiohazardRe4.h"

#include "Components/CapsuleComponent.h"

#include "DamageType/MonsterDamageType/BDMGMonsterSmallTop.h"
#include "DamageType/MonsterDamageType/BDMGMonsterLargeTop.h"
#include "DamageType/MonsterDamageType/BDMGMonsterSmallBottom.h"
#include "DamageType/MonsterDamageType/BDMGMonsterLargeBottom.h"

#include "Actor/Monster/Define/MonsterDefine.h"
#include "Actor/Monster/DataAsset/BMonsterStatData.h"
#include "Actor/Monster/Interface/BMonsterAnimInterface.h"
#include "Actor/Monster/AIController/BAIBasicMonsterController.h"

TArray<TPair<FString, TObjectPtr<USkeletalMesh>>> ABBasicMonsterBase::LoadedWeaponMesh;
ABBasicMonsterBase::ABBasicMonsterBase()
{
	Weapon = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Weapon"));
	
	LoadWeaponMesh();

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

	SetAIMode(AIMode);
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

		SetWeaponCollision(EWeaponName::Hands);
		return;
	}
	else
	{
		LOG_MSG(TEXT("Monster One Hand"));
		MyWeaponType = EWeaponType::OneHand;
	}
	
	//이름, 경로
	
	FString SocketString = LoadedWeaponMesh[WeaponIndex].Key + "Socket";
	FName SocketName = FName(SocketString);
	
	if (LoadedWeaponMesh[WeaponIndex].Value == nullptr)
	{
		LOG_WARNING(TEXT("WeaponMesh is nullptr"));
	}

	Weapon->SetSkeletalMesh(LoadedWeaponMesh[WeaponIndex].Value);
	Weapon->AttachToComponent(GetMesh(), FAttachmentTransformRules(EAttachmentRule::KeepRelative, true), SocketName);
	
	SetWeaponCollision(StaticCast<EWeaponName>(WeaponIndex));
}

void ABBasicMonsterBase::InitAI()
{
	AIControllerClass = ABAIBasicMonsterController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

void ABBasicMonsterBase::SetAIMode(EAIMode _AIMode)
{
	AController* MyController = GetController();
	AAIController* MyAIController = Cast<AAIController>(MyController);

	MyAIController->GetBlackboardComponent()->SetValueAsEnum(BBKEY_AIMODE, StaticCast<uint8>(_AIMode));
}

void ABBasicMonsterBase::SetClothesSkeletalMeshByRandomInBeginPlay()
{

}

void ABBasicMonsterBase::SetAnimInstanceAndAnimationMontageInBeginPlay()
{

}

void ABBasicMonsterBase::SetWeaponCollision(EWeaponName _WeaponName)
{
	switch (_WeaponName)
	{
	case EWeaponName::Hands:
		WeaponCollision->SetCapsuleSize(20.0f, 20.0f);
		break;
	case EWeaponName::HandAxe:
		WeaponCollision->AttachToComponent(Weapon, FAttachmentTransformRules(EAttachmentRule::KeepRelative, true), FName(TEXT("CollisionSocket")));
		WeaponCollision->SetCapsuleSize(20.0f, 20.0f);
		break;
	case EWeaponName::KitchenKnife:
		WeaponCollision->AttachToComponent(Weapon, FAttachmentTransformRules(EAttachmentRule::KeepRelative, true), FName(TEXT("CollisionSocket")));
		WeaponCollision->SetCapsuleSize(10.0f, 40.0f);
		break;
	case EWeaponName::Sickle:
		WeaponCollision->AttachToComponent(Weapon, FAttachmentTransformRules(EAttachmentRule::KeepRelative, true), FName(TEXT("CollisionSocket")));
		WeaponCollision->SetCapsuleSize(10.0f, 30.0f);
		break;
	case EWeaponName::Torch:
		WeaponCollision->AttachToComponent(Weapon, FAttachmentTransformRules(EAttachmentRule::KeepRelative, true), FName(TEXT("CollisionSocket")));
		WeaponCollision->SetCapsuleSize(20.0f, 20.0f);
		break;
	default:
		break;
	}
}

void ABBasicMonsterBase::LoadWeaponMesh()
{
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> Weapon1Ref(TEXT("/Script/Engine.SkeletalMesh'/Game/Assets/Monster/Mesh/BasicMonster/Weapon/HandAxe/SK_HandAxe.SK_HandAxe'"));
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> Weapon2Ref(TEXT("/Script/Engine.SkeletalMesh'/Game/Assets/Monster/Mesh/BasicMonster/Weapon/KitchenKnife/SK_KitchenKnife.SK_KitchenKnife'"));
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> Weapon3Ref(TEXT("/Script/Engine.SkeletalMesh'/Game/Assets/Monster/Mesh/BasicMonster/Weapon/Sickle/SK_Sickle.SK_Sickle'"));
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> Weapon4Ref(TEXT("/Script/Engine.SkeletalMesh'/Game/Assets/Monster/Mesh/BasicMonster/Weapon/Torch/SK_Torch.SK_Torch'"));
	
	TPair<FString, TObjectPtr<USkeletalMesh>> Weapon1(TEXT("HandAxe"), Weapon1Ref.Object);
	LoadedWeaponMesh.Add(Weapon1);

	TPair<FString, TObjectPtr<USkeletalMesh>> Weapon2(TEXT("KitchenKnife"), Weapon2Ref.Object);
	LoadedWeaponMesh.Add(Weapon2);

	TPair<FString, TObjectPtr<USkeletalMesh>> Weapon3(TEXT("Sickle"), Weapon3Ref.Object);
	LoadedWeaponMesh.Add(Weapon3);

	TPair<FString, TObjectPtr<USkeletalMesh>> Weapon4(TEXT("Torch"), Weapon4Ref.Object);
	LoadedWeaponMesh.Add(Weapon4);
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
	ABAIBasicMonsterController* AIController = Cast<ABAIBasicMonsterController>(GetController());
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
	AIController->Announce(TargetPawn);

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
