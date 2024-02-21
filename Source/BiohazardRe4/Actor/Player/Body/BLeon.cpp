// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Player/Body/BLeon.h"
#include "Actor/Player/HUD/HUDBase.h"
#include "InputAction.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "MathUtil.h"
#include "Components/InputComponent.h"
#include "Components/SceneComponent.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "Components/SceneCaptureComponent2D.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"
#include "Engine/DamageEvents.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "BiohazardRe4.h"
#include "../Weapon/Grenade/BLeonGrenade.h"
#include "../Weapon/Gun/BLeonPistol.h"
#include "../Weapon/Gun/BLeonRifle.h"
#include "../Weapon/Gun/BLeonShotgun.h"
#include "../Weapon/Knife/BLeonKnife.h"
#include "../Weapon/BDrawGrenadeAim.h"
#include "../../Map/BJumpObstacleTrigger.h"
#include "../../Map/BCliffLineTrigger.h"
#include "Actor/Map/BMapBaseInteraction.h"
#include "Item/InventoryActor.h"
#include "Item/InventoryWeapon.h"
#include "Item/BItem.h"
#include "MapUI/BMapUIActor.h"
#include "Actor/Monster/MonsterActor/BMonsterBase.h"
#include "DamageType/BDMGMonsterDamage.h"
#include "DamageType/PlayerDamageType/BDMGPlayerGranade.h"
#include "Generic/BFsm.h"
#include "Generic/BCollisionObserverCapsule.h"
#include "Generic/BCollisionObserverSphere.h"
#include "Actor/Merchant/Merchant.h"

const FVector ABLeon::StandSocketOffset = FVector(0.0f, 35.0f, -12.0f);
const FVector ABLeon::GunAimSocketOffset = FVector(0.0f, 35.0f, -1.0f);
const FVector ABLeon::GreanadeAimSocketOffset = FVector(0.0f, 50.0f, -12.0f);

const float ABLeon::StandSpringArmLength = 80.0f;
const float ABLeon::GunAimSpringArmLength = 30.0f;
const float ABLeon::GreanadeAimSpringArmLength = 100.0f;

// Sets default values
ABLeon::ABLeon()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CreateSprintArm();
	CreateFSM();
	GravityActive();
	GetCharacterMovement()->bCanWalkOffLedges = true;

	GrenadeThrowingLocation = CreateDefaultSubobject<USceneComponent>(TEXT("Grenade Start"));
	GrenadeThrowingLocation->SetupAttachment(GetMesh(), TEXT("R_GrenadeSocket"));
}

ABLeon::~ABLeon()
{
	if (nullptr != CurrentWeapon)
	{
		CurrentWeapon->Destroy();
		CurrentWeapon = nullptr;
	}
}

// Called when the game starts or when spawned
void ABLeon::BeginPlay()
{
	Super::BeginPlay();

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	GrenadeAimActor = GetWorld()->SpawnActor<ABDrawGrenadeAim>(SpawnParams);

	CreateCollision();

	InventoryActor = GetWorld()->SpawnActor<ABInventoryActor>(InventoryClass, InventoryTransform);

	FsmComp->ChangeState(TO_KEY(ELeonState::Idle));
}

// Called every frame
void ABLeon::Tick(float _DeltaTime)
{
	Super::Tick(_DeltaTime);
	
	SpringArmUpdate(_DeltaTime);
	UseWeaponUpdate(_DeltaTime);
	WeaponSocketUpdate(_DeltaTime);
	SocketSwapUpdate(_DeltaTime);
	HealthStateUpdate(_DeltaTime);
	InteractionUpdate(_DeltaTime);
	GunRecoilUpdate(_DeltaTime);

	InteractionCoolTime += _DeltaTime;
	LeonFSMState = GetCurrentFSMState();
}

// Called to bind functionality to input
void ABLeon::SetupPlayerInputComponent(UInputComponent* _PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(_PlayerInputComponent);

	UEnhancedInputComponent* Input = Cast<UEnhancedInputComponent>(_PlayerInputComponent);

	APlayerController* PlayerController = Cast<APlayerController>(Controller);

	if (PlayerController == nullptr)
	{
		LOG_FATAL(TEXT("Failed Find APlayerController"));
	}

	HUD = Cast<ABHUDBase>(PlayerController->GetHUD());
	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());

	if (HUD == nullptr)
	{
		LOG_ERROR(TEXT("Failed Get ABHUDBase."));
	}
	if (Subsystem == nullptr)
	{
		LOG_FATAL(TEXT("Failed Get GetSubsystem<UEnhancedInputLocalPlayerSubsystem>"));
	}

	Subsystem->AddMappingContext(DefaultMappingContext, 0);

	if (nullptr == Input)
	{
		LOG_FATAL(TEXT("Failed Find InputComponent"));
	}
	if (nullptr == MoveAction)
	{
		LOG_FATAL(TEXT("is Not Set MoveAction"));
	}
	if (nullptr == LookAction)
	{
		LOG_FATAL(TEXT("is Not Set LookAction"));
	}
	if (nullptr == JogAction)
	{
		LOG_FATAL(TEXT("is Not Set JogAction"));
	}
	if (nullptr == CrouchAction)
	{
		LOG_FATAL(TEXT("is Not Set RunAction"));
	}
	if (nullptr == AimAction)
	{
		LOG_FATAL(TEXT("is Not Set AimAction"));
	}
	if (nullptr == InteractionAction)
	{
		LOG_FATAL(TEXT("is Not Set InteractionAction"));
	}
	if (nullptr == WeaponPutAwayAction)
	{
		LOG_FATAL(TEXT("is Not Set WeaponPutAwayAction"));
	}
	if (nullptr == ShootAction)
	{
		LOG_FATAL(TEXT("is Not Set ShootAction"));
	}
	if (nullptr == WeaponReloadAction)
	{
		LOG_FATAL(TEXT("is Not Set WeaponReloadAction"));
	}
	if (nullptr == InventoryAction)
	{
		LOG_FATAL(TEXT("is Not Set InventoryAction"));
	}

	Input->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ABLeon::PlayMove);
	Input->BindAction(MoveAction, ETriggerEvent::None, this, &ABLeon::PlayIdle);
	Input->BindAction(LookAction, ETriggerEvent::Triggered, this, &ABLeon::PlayLook);
	Input->BindAction(LookAction, ETriggerEvent::None, this, &ABLeon::StopLook);

	Input->BindAction(JogAction, ETriggerEvent::Triggered, this, &ABLeon::ActiveJog);
	Input->BindAction(JogAction, ETriggerEvent::Completed, this, &ABLeon::DisableJog);
	Input->BindAction(AimAction, ETriggerEvent::Triggered, this, &ABLeon::ActiveAim);
	Input->BindAction(AimAction, ETriggerEvent::Completed, this, &ABLeon::DisableAim);

	Input->BindAction(CrouchAction, ETriggerEvent::Completed, this, &ABLeon::TryCrouch);
	Input->BindAction(InteractionAction, ETriggerEvent::Completed, this, &ABLeon::TryInteraction);

	Input->BindAction(InventoryAction, ETriggerEvent::Completed, this, &ABLeon::OpenInventory);
	Input->BindAction(MapUIAction, ETriggerEvent::Completed, this, &ABLeon::MapUISwitch);

	for (uint32 i = 0; i < 8; ++i)
	{
		if (nullptr == QuickSlotActions[i])
		{
			LOG_FATAL(TEXT("is Not Set QuickSlotActions[%d]"), i);
		}

		Input->BindAction(QuickSlotActions[i], ETriggerEvent::Started, this, &ABLeon::UseQuickSlot, i);
	}

	Input->BindAction(WeaponPutAwayAction, ETriggerEvent::Started, this, &ABLeon::ChangeUseWeapon, EItemCode::Empty);
	Input->BindAction(ShootAction, ETriggerEvent::Started, this, &ABLeon::Attack);
	Input->BindAction(WeaponReloadAction, ETriggerEvent::Started, this, &ABLeon::ReloadActive);
}

float ABLeon::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float DamageValue = Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);

	switch (LeonFSMState)
	{
	case ELeonState::KickAttack:
	case ELeonState::Damage:
	case ELeonState::Death:
	case ELeonState::ObstacleJump:
	case ELeonState::OpenGate:
	case ELeonState::Parry:
	case ELeonState::CutScene00:
		return 0.0f;
	}
	
	bool bIsSafeDamage = false;

	FVector AttackerLocation = DamageCauser->GetActorLocation();

	FsmComp->ChangeState(TO_KEY(ELeonState::Damage));
	UBDMGMonsterDamage* MonsterDamageClass = Cast<UBDMGMonsterDamage>(DamageEvent.DamageTypeClass.GetDefaultObject());

	if (nullptr != MonsterDamageClass)
	{
		Stat.CurrentHp -= DamageValue;
		HUD->SetHP(Stat.CurrentHp / Stat.MaxHp);

		if (0 >= Stat.CurrentHp)
		{
			Stat.CurrentHp = 0;
			FsmComp->ChangeState(TO_KEY(ELeonState::Death));
			return DamageValue;
		}

		double Angle = GetAxisZAngle(AttackerLocation);

		if (Angle > -45.0 && Angle <= 45.0)
		{
			// Forward
			if (EMonsterDamageDirection::Top == MonsterDamageClass->DamageDirection)
			{
				DamageDirection = ELeonDamageDirection::FT;
				LOG_MSG(TEXT("FT"));
			}
			else
			{
				DamageDirection = ELeonDamageDirection::FU;
				LOG_MSG(TEXT("FU"));
			}
		}
		else if (Angle > 45.0 && Angle <= 135.0)
		{
			// Right
			if (EMonsterDamageDirection::Top == MonsterDamageClass->DamageDirection)
			{
				DamageDirection = ELeonDamageDirection::RT;
				LOG_MSG(TEXT("RT"));
			}
			else
			{
				DamageDirection = ELeonDamageDirection::RU;
				LOG_MSG(TEXT("RU"));
			}
		}
		else if (Angle > -135.0 && Angle <= -45.0)
		{
			// Left
			if (EMonsterDamageDirection::Top == MonsterDamageClass->DamageDirection)
			{
				DamageDirection = ELeonDamageDirection::LT;
				LOG_MSG(TEXT("LT"));
			}
			else
			{
				DamageDirection = ELeonDamageDirection::LU;
				LOG_MSG(TEXT("LU"));
			}
		}
		else
		{
			// Back
			DamageDirection = ELeonDamageDirection::B;
			LOG_MSG(TEXT("B"));
		}

		switch (MonsterDamageClass->DamagePower)
		{
		case EMonsterDamagePower::Small:
		{
			DamageType = ELeonDamageType::Small;
		}
		break;
		case EMonsterDamagePower::Medium:
		{
			DamageType = ELeonDamageType::Medium;
		}
		break;
		case EMonsterDamagePower::Large:
		{
			DamageType = ELeonDamageType::Large;
		}
		break;
		case EMonsterDamagePower::ExLarge:
		{
			DamageType = ELeonDamageType::ExLarge;
		}
		break;
		}

		return DamageValue;
	}

	UBDMGPlayerGranade* GrenadeDamageClass = Cast<UBDMGPlayerGranade>(DamageEvent.DamageTypeClass.GetDefaultObject());

	if (nullptr != GrenadeDamageClass)
	{
		double Angle = GetAxisZAngle(AttackerLocation);

		if (Angle > -45.0 && Angle <= 45.0)
		{
			// Forward
			if (GetActorLocation().Z < AttackerLocation.Z)
			{
				DamageDirection = ELeonDamageDirection::FT;
				LOG_MSG(TEXT("FT"));
			}
			else
			{
				DamageDirection = ELeonDamageDirection::FU;
				LOG_MSG(TEXT("FU"));
			}
		}
		else if (Angle > 45.0 && Angle <= 135.0)
		{
			// Right
			if (GetActorLocation().Z < AttackerLocation.Z)
			{
				DamageDirection = ELeonDamageDirection::RT;
				LOG_MSG(TEXT("RT"));
			}
			else
			{
				DamageDirection = ELeonDamageDirection::RU;
				LOG_MSG(TEXT("RU"));
			}
		}
		else if (Angle > -135.0 && Angle <= -45.0)
		{
			// Left
			if (GetActorLocation().Z < AttackerLocation.Z)
			{
				DamageDirection = ELeonDamageDirection::LT;
				LOG_MSG(TEXT("LT"));
			}
			else
			{
				DamageDirection = ELeonDamageDirection::LU;
				LOG_MSG(TEXT("LU"));
			}
		}
		else
		{
			// Back
			DamageDirection = ELeonDamageDirection::B;
			LOG_MSG(TEXT("B"));
		}

		float Distance = FVector::Distance(AttackerLocation, GetActorLocation());

		if (100 > Distance)
		{
			DamageType = ELeonDamageType::Guard;
			return 0.0f;
		}
		else
		{
			DamageType = ELeonDamageType::Explosion;

			FVector ToVector = AttackerLocation - GetActorLocation();
			ToVector.Z = 0;
			ToVector.Normalize();

			SetActorRotation(ToVector.Rotation());

			Stat.CurrentHp -= DamageValue;
			HUD->SetHP(Stat.CurrentHp / Stat.MaxHp);

			if (0 >= Stat.CurrentHp)
			{
				Stat.CurrentHp = 0;
				FsmComp->ChangeState(TO_KEY(ELeonState::Death));
				return DamageValue;
			}
		}		
	}

	LOG_ERROR(TEXT("Undefined DamageType"));

	return DamageValue;
}

void ABLeon::TakeHeal(float Heal)
{
	Stat.CurrentHp = FMath::Min(Stat.CurrentHp + Heal, Stat.MaxHp);
	HUD->SetHP(Stat.CurrentHp / Stat.MaxHp);
}

FVector ABLeon::GetCameraDirection() const
{
	return PlayerCamera->GetForwardVector();
}

ELeonDirection ABLeon::GetLeonDirection() const
{
	FVector DirVector = GetInputDirection().GetSafeNormal();

	double Angle = FMath::RadiansToDegrees(FMath::Atan2(DirVector.X, DirVector.Y));

	if (Angle > -22.5 && Angle <= 22.5)
	{
		return ELeonDirection::F;
	}
	else if (Angle > 22.5 && Angle <= 67.5)
	{
		return ELeonDirection::FR;
	}
	else if (Angle > 67.5 && Angle <= 112.5)
	{
		return ELeonDirection::R;
	}
	else if (Angle > 112.5 && Angle <= 157.5)
	{
		return ELeonDirection::BR;
	}
	else if (Angle > -67.5 && Angle <= -22.5)
	{
		return ELeonDirection::FL;
	}
	else if (Angle > -112.5 && Angle <= -67.5)
	{
		return ELeonDirection::L;
	}
	else if (Angle > -157.5 && Angle <= -112.5)
	{
		return ELeonDirection::BL;
	}
	else
	{
		return ELeonDirection::B;
	}
}

ABInventoryActor* ABLeon::GetInventoryActor() const
{
	return ABInventoryActor::Instance;
}

FTransform ABLeon::GetWeaponLeftSocketTransform() const
{
	if (nullptr == CurrentWeapon)
	{
		return FTransform::Identity;
	}

	return CurrentWeapon->GetLeftHandSocketTransform();
}

void ABLeon::CutSceneDeleteWeapon()
{
	DeleteCurrentWeapon();
	UseWeaponCode = EItemCode::Empty;

	bIsGunReload = false;
	bIsGunRecoil = false;

	LeonWeaponSwap = ELeonWeaponSwap::None;
}

void ABLeon::WeaponPutOutStart()
{
	LOG_MSG(TEXT("Leon Weapon PutOutStart"));

	if (nullptr != CurrentWeapon)
	{
		DeleteCurrentWeapon();
	}

	switch (UseWeaponCode)
	{
	case EItemCode::Handgun_SR09R: // Pistol
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), PistolPutOutSound, GetActorLocation());
	}
	break;
	case EItemCode::Shotgun_W870: // Shotgun
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), ShotgunPutOutSound, GetActorLocation());
	}
	break;
	case EItemCode::Rifle_SRM1903: // Rifle
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), RiflePutOutSound, GetActorLocation());
	}
	break;
	
	case EItemCode::CombatKnife: // CombatKnife
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), KnifePutOutSound, GetActorLocation());
	}
	break;
	case EItemCode::Grenade: // Grenade
	case EItemCode::Flashbang: // FlashBang
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), GrenadePutOutSound, GetActorLocation());
	}
	break;
	}

	if (nullptr == CurrentWeapon)
	{
		CurrentWeapon = CreateWeapon(UseWeaponCode);

		if (false == bIsLerpWeaponChange)
		{
			FVector EndLocation = GetMesh()->GetSocketLocation(LerpSocketEnd);
			FRotator EndRotation = GetMesh()->GetSocketRotation(LerpSocketEnd);

			CurrentWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, LerpSocketEnd);

			CurrentWeapon->SetActorLocation(EndLocation);
			CurrentWeapon->SetActorRotation(EndRotation);
		}
		else
		{
			FVector StartLocation = GetMesh()->GetSocketLocation(LerpSocketStart);
			FRotator StartRotation = GetMesh()->GetSocketRotation(LerpSocketStart);

			CurrentWeapon->SetActorLocation(StartLocation);
			CurrentWeapon->SetActorRotation(StartRotation);

			SocketLocationLerpTime = 0.0f;
			SocketRotationLerpTime = 0.0f;
		}
		HUD->SetWeaponCode(UseWeaponCode);
	}
}

void ABLeon::WeaponPutOutEnd()
{
	LeonWeaponSwap = ELeonWeaponSwap::None;

	LOG_MSG(TEXT("Leon Weapon PutOutEnd"));
}

void ABLeon::WeaponPutAwayStart()
{
	LOG_MSG(TEXT("Leon Weapon PutAwayStart"));

	if (nullptr != CurrentWeapon)
	{
		switch (UseWeaponCode)
		{
		case EItemCode::Handgun_SR09R: // Pistol
		{
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), PistolPutAwaySound, GetActorLocation());
		}
		break;
		case EItemCode::Shotgun_W870: // Shotgun
		{
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), ShotgunPutAwaySound, GetActorLocation());
		}
		break;
		case EItemCode::Rifle_SRM1903: // Rifle
		{
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), RiflePutAwaySound, GetActorLocation());
		}
		break;

		case EItemCode::CombatKnife: // CombatKnife
		{
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), KnifePutAwaySound, GetActorLocation());
		}
		break;
		case EItemCode::Grenade: // Grenade
		case EItemCode::Flashbang: // FlashBang
		{
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), GrenadePutAwaySound, GetActorLocation());
		}
		break;
		}

		ELeonWeaponAnim WeaponAnim = GetUseWeaponAnimation(UseWeaponCode);

		SocketLocationLerpTime = 0.0f;
		SocketRotationLerpTime = 0.0f;

		switch (WeaponAnim)
		{
		case ELeonWeaponAnim::Shotgun:
		{
			LOG_MSG(TEXT("Leon Weapon DetachFromActor"));

			CurrentWeapon->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);

			LerpSocketStart = TEXT("R_ShotgunSocket");
			LerpSocketEnd = TEXT("ShotgunSpineSocket");
			bIsLerpWeaponChange = true;

			SocketLocationLerpSpeed = 3.5f;
			SocketRotationLerpSpeed = 2.0f;

			SocketLocationBlend = PutAwayLocationAlphaBlend;
			SocketRotationBlend = AbsolutAlphaBlend;
			SocketLocationBlend.SetBlendTime(1.0f / SocketLocationLerpSpeed);
			SocketRotationBlend.SetBlendTime(1.0f / SocketRotationLerpSpeed);
			SocketLocationBlend.Reset();
			SocketRotationBlend.Reset();

			SocketLocationLerpTime = 0.0f;
			SocketRotationLerpTime = 0.0f;
		}
		break;
		case ELeonWeaponAnim::Rifle:
		{
			LOG_MSG(TEXT("Leon Weapon DetachFromActor"));

			CurrentWeapon->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);

			LerpSocketStart = TEXT("R_RifleSocket");
			LerpSocketEnd = TEXT("RifleSpineSocket");
			bIsLerpWeaponChange = true;

			SocketLocationLerpSpeed = 3.5f;
			SocketRotationLerpSpeed = 2.0f;

			SocketLocationBlend = PutAwayLocationAlphaBlend;
			SocketRotationBlend = AbsolutAlphaBlend;
			SocketLocationBlend.SetBlendTime(1.0f / SocketLocationLerpSpeed);
			SocketRotationBlend.SetBlendTime(1.0f / SocketRotationLerpSpeed);
			SocketLocationBlend.Reset();
			SocketRotationBlend.Reset();

			SocketLocationLerpTime = 0.0f;
			SocketRotationLerpTime = 0.0f;
		}
		break;
		default:
			break;
		}

		if (bIsLerpWeaponChange == true)
		{
			FVector StartLocation = GetMesh()->GetSocketLocation(LerpSocketStart);
			FRotator StartRotation = GetMesh()->GetSocketRotation(LerpSocketStart);

			CurrentWeapon->SetActorLocation(StartLocation);
			CurrentWeapon->SetActorRotation(StartRotation);
		}
	}
}

void ABLeon::WeaponPutAwayEnd()
{
	UseWeaponCode = PutOutWeapon;
	PutOutWeapon = EItemCode::Empty;

	ELeonWeaponAnim CurAnimation = GetUseWeaponAnimation(UseWeaponCode);

	if (CurAnimation == ELeonWeaponAnim::Empty)
	{
		LeonWeaponSwap = ELeonWeaponSwap::None;
	}
	else
	{
		LeonWeaponSwap = ELeonWeaponSwap::PutOut;
	}

	if (nullptr != CurrentWeapon)
	{
		DeleteCurrentWeapon();
	}

	LOG_MSG(TEXT("Leon Weapon PutAwayEnd"));
}

void ABLeon::WeaponShootStart()
{

}

void ABLeon::WeaponShootEnd()
{
	bIsGunRecoil = false;
}

void ABLeon::ThrowingWeapon()
{
	bIsThrowingWeapon = true;
}

void ABLeon::ThrowingEnd()
{
	bIsThrowingEnd = true;
}

void ABLeon::KickAttackStart()
{
	bIsKickAttackActive = true;
	KickOverlapObserver->SetVisibilityCollision(true);
}

void ABLeon::KickAttackEnd()
{
	bIsKickAttackActive = false;
	KickOverlapObserver->SetVisibilityCollision(false);
}

void ABLeon::KickEnd()
{
	bIsKickEnd = true;
}

void ABLeon::ParryEnd()
{
	bIsParryEnd = true;
}

void ABLeon::AttachLeftHandSocket()
{
	if (nullptr == CurrentWeapon)
	{
		return;
	}

	FName Socket = NAME_None;

	switch (UseWeaponCode)
	{
	case EItemCode::Handgun_SR09R:
		Socket = "L_PistolSocket";
		SocketSwapLocationSpeed = 50.0f;
		SocketSwapRotationSpeed = 50.0f;
		break;
	case EItemCode::Shotgun_W870:
		Socket = "L_ShotgunSocket";
		SocketSwapLocationSpeed = 50.0f;
		SocketSwapRotationSpeed = 50.0f;
		break;
	case EItemCode::Rifle_SRM1903:
		Socket = "L_RifleSocket";
		SocketSwapLocationSpeed = 50.0f;
		SocketSwapRotationSpeed = 50.0f;
		break;
	case EItemCode::CombatKnife:
		Socket = "L_KnifeSocket";
		SocketSwapLocationSpeed = 50.0f;
		SocketSwapRotationSpeed = 50.0f;
		break;
	case EItemCode::Grenade:
		Socket = "L_GrenadeSocket";
		SocketSwapLocationSpeed = 50.0f;
		SocketSwapRotationSpeed = 50.0f;
		break;
	case EItemCode::Flashbang:
		Socket = "L_FlashbangSocket";
		SocketSwapLocationSpeed = 50.0f;
		SocketSwapRotationSpeed = 50.0f;
		break;
	}

	CurrentWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepWorldTransform, Socket);
	bIsLerpSocket = true;
}

void ABLeon::AttachRightHandSocket()
{
	if (nullptr == CurrentWeapon)
	{
		return;
	}

	FName Socket = NAME_None;

	switch (UseWeaponCode)
	{
	case EItemCode::Handgun_SR09R:
		Socket = "R_PistolSocket";
		SocketSwapLocationSpeed = 50.0f;
		SocketSwapRotationSpeed = 50.0f;
		break;
	case EItemCode::Shotgun_W870:
		Socket = "R_ShotgunSocket";
		SocketSwapLocationSpeed = 50.0f;
		SocketSwapRotationSpeed = 50.0f;
		break;
	case EItemCode::Rifle_SRM1903:
		Socket = "R_RifleSocket";

		if (true == bIsCrouch)
		{
			SocketSwapLocationSpeed = 100.0f;
			SocketSwapRotationSpeed = 360.0f;
		}
		else
		{
			SocketSwapRotationSpeed = 180.0f;
			SocketSwapLocationSpeed = 50.0f;
		}
		break;
	case EItemCode::CombatKnife:
		Socket = "R_KnifeSocket";
		SocketSwapLocationSpeed = 50.0f;
		SocketSwapRotationSpeed = 50.0f;
		break;
	case EItemCode::Grenade:
		Socket = "R_GrenadeSocket";
		SocketSwapLocationSpeed = 50.0f;
		SocketSwapRotationSpeed = 50.0f;
		break;
	case EItemCode::Flashbang:
		Socket = "R_FlashbangSocket";
		SocketSwapLocationSpeed = 50.0f;
		SocketSwapRotationSpeed = 50.0f;
		break;
	}

	CurrentWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepWorldTransform, Socket);
	bIsLerpSocket = true;
}

void ABLeon::ReloadStart()
{
	if (nullptr == CurrentWeapon)
	{
		LOG_FATAL(TEXT("Player Weapon is nullptr"));
	}

	CurrentWeapon->ReloadStart();
}

void ABLeon::Reload()
{
	if (nullptr == CurrentWeapon)
	{
		return;
	}

	CurrentWeapon->Reload();
}

void ABLeon::ReloadEnd()
{
	bIsGunReload = false;
}

void ABLeon::ChangeUseWeapon(EItemCode _WeaponCode)
{
	bIsGunReload = false;

	switch (LeonWeaponSwap)
	{
	case ELeonWeaponSwap::PutAway:
	{
		PutOutWeapon = _WeaponCode;
	}
	break;
	default:
	{
		if (UseWeaponCode == _WeaponCode)
		{
			return;
		}

		ELeonWeaponAnim CurAnimation = GetUseWeaponAnimation(UseWeaponCode);

		if (CurAnimation == ELeonWeaponAnim::Empty)
		{
			LeonWeaponSwap = ELeonWeaponSwap::PutOut;
			UseWeaponCode = _WeaponCode;
		}
		else
		{
			LeonWeaponSwap = ELeonWeaponSwap::PutAway;
			PutOutWeapon = _WeaponCode;
		}
	}
	break;
	}

}

ELeonState ABLeon::GetCurrentFSMState() const
{
	return static_cast<ELeonState>(FsmComp->GetCurrentFSMKey());
}

void ABLeon::SetCurrentFSMState(ELeonState State)
{
	FsmComp->ChangeState(TO_KEY(State));
}

void ABLeon::PlayMove(const FInputActionInstance& _MoveAction)
{
	FVector2D MoveInput2D = _MoveAction.GetValue().Get<FVector2D>();
	MoveInput = FVector(MoveInput2D.X, MoveInput2D.Y, 0);
}

void ABLeon::PlayIdle(const FInputActionInstance& _MoveAction)
{
	MoveInput = FVector::ZeroVector;
}

bool ABLeon::AbleInteraction() const
{
	if (0.0f > InteractionCoolTime)
	{
		return false;
	}

	if (true == bIsGunReload)
	{
		return false;
	}

	switch (LeonFSMState)
	{
	case ELeonState::Aim:
	case ELeonState::KnifeAttack:
	case ELeonState::KickAttack:
	case ELeonState::Throwing:
	case ELeonState::Damage:
	case ELeonState::Death:
	case ELeonState::ObstacleJump:
	case ELeonState::Fall:
	case ELeonState::OpenDoor:
	case ELeonState::Parry:
	case ELeonState::BreakBox:
		return false;
	}

	return true;
}

void ABLeon::TryInteraction()
{
	if (nullptr == InteractionObject)
	{
		return;
	}

	if (false == AbleInteraction())
	{
		return;
	}

	EInteraction InteractionType = InteractionObject->GetInteractionType();

	switch (InteractionType)
	{
	case EInteraction::None:
		break;
	case EInteraction::AttackMonster:
	{
		ABMonsterBase* MonsterActor = Cast<ABMonsterBase>(InteractionActor);

		if (nullptr == MonsterActor)
		{
			LOG_FATAL(TEXT("Only classes that inherit ABMonsterBase can have a AttackMonster Interaction Type"));
			break;
		}

		MonsterActor->Parry();

		DeleteCurrentWeapon();

		UseWeaponCode = EItemCode::CombatKnife;
		CurrentWeapon = CreateWeapon(UseWeaponCode);
		CurrentWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, LerpSocketEnd);
		FsmComp->ChangeState(TO_KEY(ELeonState::Parry));
	}
	break;
	case EInteraction::GroggyMonster:
	{
		KickLocation = InteractionActor->GetActorLocation();
		FsmComp->ChangeState(TO_KEY(ELeonState::KickAttack));
	}
	break;
	case EInteraction::BreakBox:
	{
		FVector ActorLocation = GetActorLocation();
		KickLocation = InteractionActor->GetActorLocation();

		FVector ToDirection = KickLocation - ActorLocation;
		ToDirection.Normalize();

		float Angle = FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(ToDirection, GetActorForwardVector())));

		if (ActorLocation.Z > KickLocation.Z)
		{
			Angle *= -1;
		}

		if (-15 > Angle)
		{
			BreakAnim = EBreakBoxState::Low;
		}
		else
		{
			BreakAnim = EBreakBoxState::Middle;
		}

		FsmComp->ChangeState(TO_KEY(ELeonState::BreakBox));
	}
	break;
	case EInteraction::JumpObstacle:
	{
		ABJumpObstacleTrigger* TriggerActor = Cast<ABJumpObstacleTrigger>(InteractionActor);

		if (nullptr == TriggerActor)
		{
			LOG_FATAL(TEXT("Only classes that inherit ABJumpObstacleTrigger can have a JumpObstacle Interaction Type"));
			break;
		}

		FJumpData JumpData = TriggerActor->GetJumpMetaData(GetActorLocation());

		if (false == JumpData.bAbleJump)
		{
			break;
		}

		FVector ActorForward = GetActorForwardVector();
		ActorForward.Z = 0;
		ActorForward.Normalize();

		float Angle = FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(ActorForward, JumpData.MoveVector)));

		if (45 < Angle)
		{
			break;
		}

		JumpStart = JumpData.Start;
		JumpEnd = JumpData.End;
		JumpDir = JumpData.MoveVector;

		FsmComp->ChangeState(TO_KEY(ELeonState::ObstacleJump));
	}
	break;
	case EInteraction::FallCliff:
	{
		ABCliffLineTrigger* TriggerActor = Cast<ABCliffLineTrigger>(InteractionActor);

		if (nullptr == TriggerActor)
		{
			LOG_FATAL(TEXT("Only classes that inherit ABCliffLineTrigger can have a FallCliff Interaction Type"));
			break;
		}

		FJumpData JumpData = TriggerActor->GetJumpMetaData(GetActorLocation());

		if (false == JumpData.bAbleJump)
		{
			break;
		}

		FVector ActorForward = GetActorForwardVector();
		ActorForward.Z = 0;
		ActorForward.Normalize();

		float Angle = FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(ActorForward, JumpData.MoveVector)));

		if (45 < Angle)
		{
			break;
		}

		JumpStart = JumpData.Start;
		JumpEnd = JumpData.End;
		JumpDir = JumpData.MoveVector;

		if (250 < JumpStart.Z - JumpEnd.Z)
		{
			FallAnimation = ELeonFallAnimation::HighHeight;
		}
		else
		{
			FallAnimation = ELeonFallAnimation::LowHeight;
		}

		FsmComp->ChangeState(TO_KEY(ELeonState::Fall));
	}
	break;
	case EInteraction::OpenDrawer:	{
		ABMapBaseInteraction* DoorInteraction = Cast<ABMapBaseInteraction>(InteractionActor);
		DoorInteraction->MapObjOpen(GetActorLocation());

		switch (LeonFSMState)
		{
		case ELeonState::Idle:
		case ELeonState::Walk:
		case ELeonState::Jog:
		{
			bIsPlayGetItem = true;
		}
			break;
		}
	}
	break;
	case EInteraction::OpenGate:
	{
		ABMapBaseInteraction* DoorInteraction = Cast<ABMapBaseInteraction>(InteractionActor);
		DoorInteraction->MapObjOpen(GetActorLocation());

		GateForward = DoorInteraction->ATrigger->GetForwardVector();
		FsmComp->ChangeState(TO_KEY(ELeonState::OpenGate));
	}
	break;
	case EInteraction::OpenDoor:
	{
		ABMapBaseInteraction* DoorInteraction = Cast<ABMapBaseInteraction>(InteractionActor);
		DoorInteraction->MapObjOpen(GetActorLocation());
		FsmComp->ChangeState(TO_KEY(ELeonState::OpenDoor));
	}
	break;
	case EInteraction::DropItem:
	{
		ABItem* Item = Cast<ABItem>(InteractionObject);

		if (nullptr == Item || false == Item->bCanPickup)
		{
			break;
		}

		Item->PickUp();

		switch (LeonFSMState)
		{
		case ELeonState::Idle:
		case ELeonState::Walk:
		case ELeonState::Jog:
		{
			bIsPlayGetItem = true;
		}
			break;
		}
	}
	break;
	case EInteraction::StoreEnter:
	{
		if (ABMerchant* Merchant = Cast<ABMerchant>(InteractionObject))
		{
			Merchant->OpenStore();
		}
	}
	break;
	default:
	{
		LOG_ERROR(TEXT("Wrong Type Interaction"));
	}
	break;
	}

	InteractionObject = nullptr;
	InteractionActor = nullptr;
}

bool ABLeon::AbleAim() const
{
	if (true == bIsGunReload)
	{
		return false;
	}

	if (LeonWeaponSwap != ELeonWeaponSwap::None)
	{
		return false;
	}

	if (LeonWeaponState == ELeonWeaponAnim::Empty)
	{
		return false;
	}

	if (LeonWeaponState == ELeonWeaponAnim::Knife)
	{
		return false;
	}

	return true;
}

void ABLeon::OpenInventory()
{
	SceneCapture->CaptureScene();
	ABInventoryActor::Instance->OpenInventory();
}

void ABLeon::MapUISwitch()
{
	if (ABMapUIActor::MapUIInst == nullptr)
	{
		LOG_ERROR(TEXT("MapUIInst == nullptr"))
		return;
	}

	ABMapUIActor::MapUIInst->MapUISwitch();
}

void ABLeon::DrawGrenadeAim(float _DeltaTime)
{
	ThrowVelocity = PlayerCamera->GetForwardVector() + FVector(0, 0, 0.5);
	ThrowVelocity.Normalize();

	FVector ActorForward = GetActorForwardVector();
	ActorForward.Normalize();

	double Angle = FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(ActorForward, ThrowVelocity)));

	if (0 > ThrowVelocity.Z)
	{
		Angle *= -1.0f;
	}

	ThrowVelocity *= GrenadeThrowingPower;

	ThrowLocation = GetGrenadeStartLocation(Angle);

	FPredictProjectilePathParams PredictParams(5.0f, ThrowLocation, ThrowVelocity, 2.0f, ECollisionChannel::ECC_GameTraceChannel11);
	PredictParams.DrawDebugType = EDrawDebugTrace::Type::None;
	PredictParams.OverrideGravityZ = GetWorld()->GetGravityZ();
	FPredictProjectilePathResult Result;
	UGameplayStatics::PredictProjectilePath(this, PredictParams, Result);

	DrawDebugSphere(GetWorld(), Result.HitResult.Location, 10, 30, FColor::Red);

	if (0 < Angle)
	{
		ThrowingAnim = ELeonThrowingAnim::Top;
		GrenadeAimActor->Draw(Result, true);
	}
	else
	{
		ThrowingAnim = ELeonThrowingAnim::Bottom;
		GrenadeAimActor->Draw(Result, true);
	}

}

FVector ABLeon::GetGrenadeStartLocation(float _Angle) const
{
	FVector StartLocation = GrenadeThrowingLocation->GetComponentLocation();

	if (0 < _Angle)
	{
		StartLocation += -GetActorForwardVector() * 8;
		StartLocation -= GetActorRightVector() * 5;
		StartLocation += FVector::UpVector * 5;
	}
	else
	{
		//StartLocation += -GetActorForwardVector();
		StartLocation -= GetActorRightVector() * 5;
		StartLocation += FVector::DownVector * 50;
	}

	return StartLocation;
}

void ABLeon::PlayLook(const FInputActionInstance& _LookAction)
{
	PlayLook(_LookAction.GetValue().Get<FVector2D>());
}

void ABLeon::PlayLook(const FVector2D& _LookAction)
{
	if (Controller != nullptr)
	{
		LookInput.X = _LookAction.X;
		LookInput.Y = _LookAction.Y;

		LookInput.X = FMath::Min(LookInput.X, MaxTurnSpeed);
		LookInput.Y = FMath::Min(LookInput.Y, MaxTurnSpeed);

		float TurnX = LookInput.X * TurnSpeed * GetWorld()->GetDeltaSeconds();
		float TurnY = LookInput.Y * TurnSpeed * GetWorld()->GetDeltaSeconds();

		AddControllerYawInput(TurnX);
		AddControllerPitchInput(TurnY);
	}
}


void ABLeon::StopLook()
{
	LookInput = FVector::ZeroVector;
}

void ABLeon::SpringArmUpdate(float _DeltaTime)
{
	if (ELeonState::Aim == LeonFSMState || true == bIsGunRecoil)
	{
		ELeonWeaponAnim WeaponAnim = GetUseWeaponAnimation(UseWeaponCode);

		switch (WeaponAnim)
		{
		case ELeonWeaponAnim::Empty:
		{
			SpringArm->SocketOffset = FMath::VInterpConstantTo(SpringArm->SocketOffset, StandSocketOffset, _DeltaTime, 200.0f);
			SpringArm->TargetArmLength = FMath::FInterpConstantTo(SpringArm->TargetArmLength, StandSpringArmLength, _DeltaTime, 200.0f);
		}
		break;
		case ELeonWeaponAnim::Pistol:
		case ELeonWeaponAnim::Shotgun:
		case ELeonWeaponAnim::Rifle:
		{
			SpringArm->SocketOffset = FMath::VInterpConstantTo(SpringArm->SocketOffset, GunAimSocketOffset, _DeltaTime, 400.0f);
			SpringArm->TargetArmLength = FMath::FInterpConstantTo(SpringArm->TargetArmLength, GunAimSpringArmLength, _DeltaTime, 400.0f);
		}
		break;
		case ELeonWeaponAnim::Knife:
		case ELeonWeaponAnim::Grenade:
		{
			SpringArm->SocketOffset = FMath::VInterpConstantTo(SpringArm->SocketOffset, GreanadeAimSocketOffset, _DeltaTime, 200.0f);
			SpringArm->TargetArmLength = FMath::FInterpConstantTo(SpringArm->TargetArmLength, GreanadeAimSpringArmLength, _DeltaTime, 200.0f);
		}
		break;
		default:
		{
			LOG_ERROR(TEXT("Player Wrong Type UseWeapon Code"));
		}
		break;
		}
	}
	else
	{
		SpringArm->SocketOffset = FMath::VInterpConstantTo(SpringArm->SocketOffset, StandSocketOffset, _DeltaTime, 200.0f);
		SpringArm->TargetArmLength = FMath::FInterpConstantTo(SpringArm->TargetArmLength, StandSpringArmLength, _DeltaTime, 200.0f);
	}
}

void ABLeon::UseWeaponUpdate(float _DeltaTime)
{
	LeonWeaponState = GetUseWeaponAnimation(UseWeaponCode);
}

void ABLeon::WeaponSocketUpdate(float _DeltaTime)
{
	if (false == bIsLerpWeaponChange)
	{
		return;
	}

	if (nullptr == CurrentWeapon)
	{
		bIsLerpWeaponChange = false;
		SocketLocationLerpTime = 0.0f;
		SocketRotationLerpTime = 0.0f;
		return;
	}

	SocketLocationBlend.Update(_DeltaTime);
	SocketRotationBlend.Update(_DeltaTime);

	SocketLocationLerpTime += _DeltaTime * SocketLocationLerpSpeed * SocketLocationBlend.GetBlendedValue();
	SocketRotationLerpTime += _DeltaTime * SocketRotationLerpSpeed * SocketRotationBlend.GetBlendedValue();

	FVector StartLocation = GetMesh()->GetSocketLocation(LerpSocketStart);
	FRotator StartRotation = GetMesh()->GetSocketRotation(LerpSocketStart);

	FVector EndLocation = GetMesh()->GetSocketLocation(LerpSocketEnd);
	FRotator EndRotation = GetMesh()->GetSocketRotation(LerpSocketEnd);

	SocketLocationLerpTime = FMath::Min(1.0f, SocketLocationLerpTime);
	SocketRotationLerpTime = FMath::Min(1.0f, SocketRotationLerpTime);

	if (1.0f <= SocketLocationLerpTime && 1.0f <= SocketRotationLerpTime)
	{
		SocketLocationLerpTime = 0.0f;
		SocketRotationLerpTime = 0.0f;

		bIsLerpWeaponChange = false;

		CurrentWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepWorldTransform, LerpSocketEnd);
		CurrentWeapon->SetActorLocation(EndLocation);
		CurrentWeapon->SetActorRotation(EndRotation);

		return;
	}

	CurrentWeapon->SetActorLocation(FMath::Lerp(StartLocation, EndLocation, SocketLocationLerpTime));
	CurrentWeapon->SetActorRotation(FMath::Lerp(StartRotation, EndRotation, SocketRotationLerpTime));
}

void ABLeon::SocketSwapUpdate(float _DeltaTime)
{
	if (nullptr == CurrentWeapon)
	{
		bIsLerpSocket = false;
	}

	if (true == bIsLerpSocket)
	{
		FVector WeaponLocation = CurrentWeapon->GetRootComponent()->GetRelativeLocation();
		FRotator WeaponRotation = CurrentWeapon->GetRootComponent()->GetRelativeRotation();
		CurrentWeapon->SetActorRelativeLocation(FMath::VInterpConstantTo(WeaponLocation, FVector::ZeroVector, _DeltaTime, SocketSwapLocationSpeed));
		CurrentWeapon->SetActorRelativeRotation(FMath::RInterpConstantTo(WeaponRotation, FRotator::ZeroRotator, _DeltaTime, SocketSwapRotationSpeed));

		WeaponLocation = CurrentWeapon->GetRootComponent()->GetRelativeLocation();
		WeaponRotation = CurrentWeapon->GetRootComponent()->GetRelativeRotation();

		if (WeaponLocation == FVector::ZeroVector && WeaponRotation == FRotator::ZeroRotator)
		{
			bIsLerpSocket = false;
		}
	}
}

void ABLeon::HealthStateUpdate(float _DeltaTime)
{
	if (0.15f <= Stat.CurrentHp / Stat.MaxHp)
	{
		LeonHealth = ELeonHealth::Normal;
	}
	else
	{
		LeonHealth = ELeonHealth::Danger;
	}
}

void ABLeon::InteractionUpdate(float _DeltaTime)
{
	InteractionActor = nullptr;
	InteractionObject = nullptr;

	if (false == AbleInteraction())
	{
		return;
	}

	TArray<AActor*> Overlaps;
	InteractionObserver->GetOverlappingActors(Overlaps);

	FVector ActorLocation = GetActorLocation();

	Overlaps.Sort([ActorLocation](const AActor& _Lhs, const AActor& _Rhs)
		{
			float DistanceL = FVector::Distance(ActorLocation, _Lhs.GetActorLocation());
			float DistanceR = FVector::Distance(ActorLocation, _Rhs.GetActorLocation());

			return DistanceL > DistanceR;
		});

	for (size_t i = 0; i < Overlaps.Num(); i++)
	{
		IBInteraction* Interface = Cast<IBInteraction>(Overlaps[i]);

		if (nullptr == Interface)
		{
			continue;
		}

		if (false == Interface->AbleInteraction())
		{
			continue;
		}

		EInteraction InteractionType = Interface->GetInteractionType();

		switch (InteractionType)
		{
		case EInteraction::None:
			continue;
		case EInteraction::DropItem: 
		{
			if (true == bIsGunReload)
			{
				continue;
			}

			if (true == bIsGunReload)
			{
				continue;
			}
		}
		break;
		case EInteraction::BreakBox:
		{
			KickLocation = Overlaps[i]->GetActorLocation();

			float Dis = FVector::Distance(KickLocation, GetActorLocation());

			if (125 < Dis)
			{
				continue;
			}

			FVector ToDirection = KickLocation - ActorLocation;
			ToDirection.Normalize();

			float Angle = FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(ToDirection, GetActorForwardVector())));

			if (ActorLocation.Z > KickLocation.Z)
			{
				Angle *= -1;
			}

			if (20 < Angle)
			{
				continue;
			}
		}
		break;
		case EInteraction::JumpObstacle:
		{
			ABJumpObstacleTrigger* TriggerActor = Cast<ABJumpObstacleTrigger>(Overlaps[i]);

			if (nullptr == TriggerActor)
			{
				LOG_FATAL(TEXT("Only classes that inherit ABJumpObstacleTrigger can have a JumpObstacle Interaction Type"));
				continue;
			}

			FJumpData JumpData = TriggerActor->GetJumpMetaData(GetActorLocation());

			if (false == JumpData.bAbleJump)
			{
				continue;
			}

			float Dis = FVector::Distance(JumpData.Start, GetActorLocation());

			if (50 < Dis)
			{
				continue;
			}

			FVector ActorForward = GetActorForwardVector();
			ActorForward.Z = 0;
			ActorForward.Normalize();

			float Angle = FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(ActorForward, JumpData.MoveVector)));

			if (45 < Angle)
			{
				continue;
			}
		}
		break;
		case EInteraction::FallCliff:
		{
			ABCliffLineTrigger* TriggerActor = Cast<ABCliffLineTrigger>(Overlaps[i]);

			if (nullptr == TriggerActor)
			{
				LOG_FATAL(TEXT("Only classes that inherit ABCliffLineTrigger can have a FallCliff Interaction Type"));
				continue;
			}

			FJumpData JumpData = TriggerActor->GetJumpMetaData(GetActorLocation());

			if (false == JumpData.bAbleJump)
			{
				continue;
			}

			float Dis = FVector::Distance(JumpData.Start, GetActorLocation());

			if (50 < Dis)
			{
				continue;
			}

			FVector ActorForward = GetActorForwardVector();
			ActorForward.Z = 0;
			ActorForward.Normalize();

			float Angle = FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(ActorForward, JumpData.MoveVector)));

			if (45 < Angle)
			{
				continue;
			}
		}
		break;
		case EInteraction::OpenDrawer:
		{
			switch (LeonFSMState)
			{
			case ELeonState::Jog:
				continue;
			}

			ABMapBaseInteraction* DoorInteraction = Cast<ABMapBaseInteraction>(Overlaps[i]);

			float Dis = FVector::Distance(DoorInteraction->ATrigger->GetComponentLocation(), GetActorLocation());

			if (50 < Dis)
			{
				continue;
			}

			FVector ActorForward = GetActorForwardVector();
			ActorForward.Z = 0;
			ActorForward.Normalize();

			float Angle = FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(ActorForward, DoorInteraction->ATrigger->GetForwardVector())));

			if (45 < Angle)
			{
				continue;
			}
		}
		break;
		case EInteraction::OpenGate:
		{
			ABMapBaseInteraction* DoorInteraction = Cast<ABMapBaseInteraction>(Overlaps[i]);

			float Dis = FVector::Distance(DoorInteraction->ATrigger->GetComponentLocation(), GetActorLocation());

			if (50 < Dis)
			{
				continue;
			}

			FVector ActorForward = GetActorForwardVector();
			ActorForward.Z = 0;
			ActorForward.Normalize();

			float Angle = FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(ActorForward, DoorInteraction->ATrigger->GetForwardVector())));

			if (45 < Angle)
			{
				continue;
			}
		}
		break;
		case EInteraction::OpenDoor:
		{
			// Todo : OpenDoor
			ABMapBaseInteraction* DoorInteraction = Cast<ABMapBaseInteraction>(Overlaps[i]);

			FVector DirectionVector = DoorInteraction->ATrigger->GetComponentLocation() - GetActorLocation();
			DirectionVector.Z = 0;
			DirectionVector.Normalize();
			FVector ActorForward = GetActorForwardVector();
			ActorForward.Z = 0;
			ActorForward.Normalize();

			float Angle = FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(ActorForward,DirectionVector)));

			if (45 < Angle)
			{
				continue;
			}

			float Dis = FVector::Distance(DoorInteraction->ATrigger->GetComponentLocation(), GetActorLocation());

			if (50 < Dis)
			{
				continue;
			}
		}
		break;
		case EInteraction::StoreEnter:
		{
			// Store Enter
		}
		break;
		default:
		break;
		}

		InteractionActor = Overlaps[i];
		InteractionObject = Interface;
		InteractionObject->Execute_EnableInteractionUI(Cast<UObject>(Interface));
		return;
	}
}

void ABLeon::GunRecoilUpdate(float _DeltaTime)
{
	if (false == bIsCameraRecoil)
	{
		return;
	}

	GunRecoilAlpha += _DeltaTime * 20.0f;

	if (3 < GunRecoilAlpha)
	{
		bIsCameraRecoil = false;
		GunRecoilAlpha = 3;
	}

	switch (GunRecoilType)
	{
	case EGunRecoilType::Pistol:
		AddControllerYawInput(FMath::Cos(GunRecoilAlpha) * 10 * _DeltaTime);
		AddControllerPitchInput(-FMath::Cos(GunRecoilAlpha) * 25 * _DeltaTime);
		break;
	case EGunRecoilType::Shotgun:
		AddControllerYawInput(FMath::Cos(GunRecoilAlpha) * 16 * _DeltaTime);
		AddControllerPitchInput(-FMath::Cos(GunRecoilAlpha) * 60 * _DeltaTime);
		break;
	case EGunRecoilType::Rifle:
		AddControllerYawInput(FMath::Cos(GunRecoilAlpha) * 20 * _DeltaTime);
		AddControllerPitchInput(-FMath::Cos(GunRecoilAlpha) * 45 * _DeltaTime);
		break;
	default:
		break;
	}

}

void ABLeon::VPlayerCameraToWorld(FVector& _Vector) const
{
	FVector CameraLook = PlayerCamera->GetForwardVector();
	CameraLook.Z = 0.0;
	CameraLook = CameraLook.GetSafeNormal();

	_Vector.X = -_Vector.X;

	double CameraAngle = FMath::Atan2(CameraLook.X, CameraLook.Y);
	_Vector = _Vector.RotateAngleAxisRad(CameraAngle, FVector::DownVector);
}

double ABLeon::GetMoveAngle() const
{
	FVector CameraForward = PlayerCamera->GetForwardVector();
	CameraForward.Z = 0.0f;

	FVector PlayerForward = GetActorForwardVector();
	PlayerForward.Z = 0.0f;

	float Angle = FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(CameraForward.GetSafeNormal(), PlayerForward.GetSafeNormal())));

	return Angle;
}

double ABLeon::GetInputAngle() const
{
	FVector InputVector = MoveInput;
	VPlayerCameraToWorld(InputVector);

	FVector PlayerForward = GetActorForwardVector();

	return FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(PlayerForward, InputVector)));
}

void ABLeon::DebugGrenade()
{
	ChangeUseWeapon(EItemCode::Grenade);
}

void ABLeon::DebugFlashBang()
{
	ChangeUseWeapon(EItemCode::Flashbang);
}

void ABLeon::JogLookAt(float _DeltaTime)
{
	FVector JogInput = MoveDir;
	VPlayerCameraToWorld(JogInput);
	FRotator InputRotator = JogInput.Rotation();

	SetActorRotation(FMath::RInterpConstantTo(GetActorRotation(), InputRotator, _DeltaTime, 360.0f));
}

bool ABLeon::AbleKnifeAttack() const
{
	if (LeonFSMState == ELeonState::Damage)
	{
		return false;
	}

	if (LeonFSMState == ELeonState::Death)
	{
		return false;
	}

	if (LeonWeaponSwap != ELeonWeaponSwap::None)
	{
		return false;
	}

	if (true == bIsGunRecoil)
	{
		return false;
	}

	if (true == bIsGunReload)
	{
		return false;
	}

	switch (LeonFSMState)
	{
	case ELeonState::Idle:
	case ELeonState::Walk:
	case ELeonState::Jog:
		return true;
	}

	return false;
}

bool ABLeon::AbleShoot() const
{
	if (LeonFSMState == ELeonState::Damage)
	{
		return false;
	}

	if (LeonFSMState == ELeonState::Death)
	{
		return false;
	}

	if (LeonFSMState != ELeonState::Aim)
	{
		return false;
	}

	if (LeonAim != ELeonAim::Loop)
	{
		return false;
	}

	if (true == bIsGunRecoil)
	{
		return false;
	}

	if (nullptr == CurrentWeapon)
	{
		return false;
	}

	return CurrentWeapon->AbleAttack();
}

void ABLeon::KnifeComboCheck()
{
	if (false == bAbleComboInput)
	{
		return;
	}

	if (true == bAbleNextCombo)
	{
		switch (KnifeAttackState)
		{
		case ELeonKnifeAttackState::EnterAttack:
		{
			KnifeAttackState = ELeonKnifeAttackState::LeftAttack;
		}
		break;
		case ELeonKnifeAttackState::LeftAttack:
		{
			KnifeAttackState = ELeonKnifeAttackState::RightAttack;
		}
		break;
		case ELeonKnifeAttackState::RightAttack:
		{
			KnifeAttackState = ELeonKnifeAttackState::LeftAttack;
		}
		break;
		case ELeonKnifeAttackState::ResetAttack:
		{
			KnifeAttackState = ELeonKnifeAttackState::LeftAttack;
		}
		break;
		}
	}
	else
	{
		switch (KnifeAttackState)
		{
		case ELeonKnifeAttackState::ResetAttack:
			KnifeAttackState = ELeonKnifeAttackState::EnterAttack;
			break;
		default:
			KnifeAttackState = ELeonKnifeAttackState::ResetAttack;
			break;
		}
	}
}

void ABLeon::Attack()
{
	LOG_MSG(TEXT("Try Player Weapon Shoot"));

	if (true == AbleShoot())
	{
		bIsWeaponShootTrigger = true;
		return;
	}
	else if (true == AbleKnifeAttack())
	{
		DeleteCurrentWeapon();
		UseWeaponCode = EItemCode::CombatKnife;
		CurrentWeapon = CreateWeapon(UseWeaponCode);
		CurrentWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, LerpSocketEnd);
		FsmComp->ChangeState(TO_KEY(ELeonState::KnifeAttack));
		return;
	}

	if (ELeonState::KnifeAttack == LeonFSMState)
	{
		KnifeComboCheck();
	}
}

void ABLeon::Aim(float _DeltaTime)
{
	FVector CameraForward = PlayerCamera->GetForwardVector();
	CameraForward.Z = 0.0;
	FRotator CameraRotator = CameraForward.Rotation();

	SetActorRotation(FMath::RInterpConstantTo(GetActorRotation(), CameraRotator, _DeltaTime, 360.0f));
}

bool ABLeon::AbleReload() const
{
	if (nullptr == CurrentWeapon)
	{
		return false;
	}

	if (false == CurrentWeapon->AbleReload())
	{
		return false;
	}

	if (LeonWeaponSwap != ELeonWeaponSwap::None)
	{
		return false;
	}

	if (true == bIsGunRecoil)
	{
		return false;
	}

	ELeonWeaponAnim WeaponAnim = GetUseWeaponAnimation(UseWeaponCode);

	switch (WeaponAnim)
	{
		// Gun 
	case ELeonWeaponAnim::Pistol:
	case ELeonWeaponAnim::Shotgun:
	case ELeonWeaponAnim::Rifle:
		return true;
	default:
		return false;
	}

	return true;
}

void ABLeon::ShotgunReloadAnimStart()
{
	ABLeonShotgun* ShotgunClass = Cast<ABLeonShotgun>(CurrentWeapon);

	if (nullptr == ShotgunClass)
	{
		return;
	}

	ShotgunClass->ReloadLoopStart();
}

void ABLeon::KnifeComboStart()
{
	bAbleComboInput = true;
	bAbleNextCombo = true;
}

void ABLeon::KnifeComboEnd()
{
	bAbleNextCombo = false;
}

void ABLeon::KnifeAttackStart()
{
	bAbleComboInput = false;
	bAbleNextCombo = false;
}

void ABLeon::KnifeAttackEnd()
{
	bIsComboEnd = true;
	bAbleComboInput = false;
}

void ABLeon::KnifeCollisionActive()
{
	if (nullptr == CurrentWeapon)
	{
		return;
	}

	CurrentWeapon->ActiveCollision(true);
}

void ABLeon::KnifeCollisionDisable()
{
	if (nullptr == CurrentWeapon)
	{
		return;
	}

	CurrentWeapon->ActiveCollision(false);
}

void ABLeon::DamageEnd()
{
	bIsHitEnd = true;
}

EInteraction ABLeon::GetInteractionType() const
{
	switch (LeonFSMState)
	{
	case ELeonState::Walk:
		return EInteraction::WalkPlayer;
	case ELeonState::Jog:
		return EInteraction::JogPlayer;
	default:
		return EInteraction::None;
	}
}

FVector ABLeon::GetUIPivot() const
{
	return FVector::ZeroVector;
}

void ABLeon::GetItemEnd()
{
	bIsPlayGetItem = false;
}

void ABLeon::JumpObstacleEnd()
{
	JumpState = ELeonJumpState::End;
}

void ABLeon::FallGravityActive()
{
	GetCharacterMovement()->GravityScale = 4.0f;
	GetCapsuleComponent()->SetCollisionProfileName("PlayerCollision");
	GetMesh()->SetCollisionProfileName("PlayerOverlap");
}

void ABLeon::FallTraceActive()
{
	bIsFallEndCheck = true;
}

void ABLeon::FallLandingEnd()
{
	bIsFallLandingEnd = true;
}

void ABLeon::GravityActive()
{
	GetCharacterMovement()->GravityScale = 4.0f;
	GetCharacterMovement()->MovementMode = EMovementMode::MOVE_Walking;
	GetCapsuleComponent()->SetCollisionProfileName("PlayerCollision");
	GetMesh()->SetCollisionProfileName("PlayerOverlap");
}

void ABLeon::DoorOpenEnd()
{
	bIsOpenEnd = true;
}

void ABLeon::ReloadActive()
{
	if (false == AbleReload())
	{
		return;
	}

	bIsGunReload = true;
}

void ABLeon::ActiveJog()
{
	bIsJogTrigger = true;
}

void ABLeon::DisableJog()
{
	bIsJogTrigger = false;
}

void ABLeon::ActiveAim()
{
	bIsAim = true;
}

void ABLeon::DisableAim()
{
	bIsAim = false;
}

void ABLeon::TryCrouch()
{
	int32 FSMKey = FsmComp->GetCurrentFSMKey();
	ELeonState FSMState = static_cast<ELeonState>(FSMKey);

	if (LeonWeaponSwap != ELeonWeaponSwap::None)
	{
		return;
	}

	if (true == bIsGunReload)
	{
		return;
	}

	if (FSMState == ELeonState::Jog)
	{
		bIsCrouch = false;
		return;
	}

	if (FSMState == ELeonState::Aim)
	{
		bIsCrouch = false;
		return;
	}

	if (FSMState == ELeonState::KnifeAttack)
	{
		bIsCrouch = false;
		return;
	}

	if (FSMState == ELeonState::KickAttack)
	{
		bIsCrouch = false;
		return;
	}

	bIsCrouch = ~bIsCrouch;
}

void ABLeon::CreateSprintArm()
{
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
	SpringArm->SetupAttachment(GetMesh(), TEXT("SpringArmSocket"));
	SpringArm->PrimaryComponentTick.bCanEverTick = true;
	SpringArm->TargetArmLength = StandSpringArmLength;
	SpringArm->bUsePawnControlRotation = true;
	SpringArm->bInheritPitch = true;
	SpringArm->bInheritYaw = true;
	SpringArm->bInheritRoll = true;
	SpringArm->bEnableCameraLag = true;
	SpringArm->CameraLagSpeed = 10.0f;
	SpringArm->bEnableCameraRotationLag = true;
	SpringArm->CameraRotationLagSpeed = 10.0f;
	SpringArm->SocketOffset = StandSocketOffset;
	SpringArm->ProbeChannel = ECollisionChannel::ECC_GameTraceChannel5;

	bUseControllerRotationRoll = false;
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;

	PlayerCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Player Camera"));
	PlayerCamera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);

	SceneCapture = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("SceneCapture"));
	SceneCapture->SetupAttachment(PlayerCamera);
}

void ABLeon::CreateFSM()
{
	FsmComp = CreateDefaultSubobject<UBFsm>(TEXT("FSM Component"));

	UBFsm::FStateCallback StandUpFSMState;
	StandUpFSMState.EnterDel.BindUObject(this, &ABLeon::IdleEnter);
	StandUpFSMState.UpdateDel.BindUObject(this, &ABLeon::IdleUpdate);
	StandUpFSMState.ExitDel.BindUObject(this, &ABLeon::IdleExit);
	FsmComp->CreateState(TO_KEY(ELeonState::Idle), StandUpFSMState);

	UBFsm::FStateCallback WalkFSMState;
	WalkFSMState.EnterDel.BindUObject(this, &ABLeon::WalkEnter);
	WalkFSMState.UpdateDel.BindUObject(this, &ABLeon::WalkUpdate);
	WalkFSMState.ExitDel.BindUObject(this, &ABLeon::WalkExit);
	FsmComp->CreateState(TO_KEY(ELeonState::Walk), WalkFSMState);

	UBFsm::FStateCallback JogMoveFSMState;
	JogMoveFSMState.EnterDel.BindUObject(this, &ABLeon::JogEnter);
	JogMoveFSMState.UpdateDel.BindUObject(this, &ABLeon::JogUpdate);
	JogMoveFSMState.ExitDel.BindUObject(this, &ABLeon::JogExit);
	FsmComp->CreateState(TO_KEY(ELeonState::Jog), JogMoveFSMState);

	UBFsm::FStateCallback AimFSMState;
	AimFSMState.EnterDel.BindUObject(this, &ABLeon::AimEnter);
	AimFSMState.UpdateDel.BindUObject(this, &ABLeon::AimUpdate);
	AimFSMState.ExitDel.BindUObject(this, &ABLeon::AimExit);
	FsmComp->CreateState(TO_KEY(ELeonState::Aim), AimFSMState);

	UBFsm::FStateCallback KnifeAttackFSMState;
	KnifeAttackFSMState.EnterDel.BindUObject(this, &ABLeon::KnifeAttackEnter);
	KnifeAttackFSMState.UpdateDel.BindUObject(this, &ABLeon::KnifeAttackUpdate);
	KnifeAttackFSMState.ExitDel.BindUObject(this, &ABLeon::KnifeAttackExit);
	FsmComp->CreateState(TO_KEY(ELeonState::KnifeAttack), KnifeAttackFSMState);

	UBFsm::FStateCallback KickAttackFSMState;
	KickAttackFSMState.EnterDel.BindUObject(this, &ABLeon::KickAttackEnter);
	KickAttackFSMState.UpdateDel.BindUObject(this, &ABLeon::KickAttackUpdate);
	KickAttackFSMState.ExitDel.BindUObject(this, &ABLeon::KickAttackExit);
	FsmComp->CreateState(TO_KEY(ELeonState::KickAttack), KickAttackFSMState);

	UBFsm::FStateCallback ThrowingFSMState;
	ThrowingFSMState.EnterDel.BindUObject(this, &ABLeon::ThrowingEnter);
	ThrowingFSMState.UpdateDel.BindUObject(this, &ABLeon::ThrowingUpdate);
	ThrowingFSMState.ExitDel.BindUObject(this, &ABLeon::ThrowingExit);
	FsmComp->CreateState(TO_KEY(ELeonState::Throwing), ThrowingFSMState);

	UBFsm::FStateCallback DamageFSMState;
	DamageFSMState.EnterDel.BindUObject(this, &ABLeon::DamageEnter);
	DamageFSMState.UpdateDel.BindUObject(this, &ABLeon::DamageUpdate);
	DamageFSMState.ExitDel.BindUObject(this, &ABLeon::DamageExit);
	FsmComp->CreateState(TO_KEY(ELeonState::Damage), DamageFSMState);

	UBFsm::FStateCallback DeathFSMState;
	DeathFSMState.EnterDel.BindUObject(this, &ABLeon::DeathEnter);
	DeathFSMState.UpdateDel.BindUObject(this, &ABLeon::DeathUpdate);
	DeathFSMState.ExitDel.BindUObject(this, &ABLeon::DeathExit);
	FsmComp->CreateState(TO_KEY(ELeonState::Death), DeathFSMState);

	UBFsm::FStateCallback JumpObstacleFSMState;
	JumpObstacleFSMState.EnterDel.BindUObject(this, &ABLeon::JumpObstacleEnter);
	JumpObstacleFSMState.UpdateDel.BindUObject(this, &ABLeon::JumpObstacleUpdate);
	JumpObstacleFSMState.ExitDel.BindUObject(this, &ABLeon::JumpObstacleExit);
	FsmComp->CreateState(TO_KEY(ELeonState::ObstacleJump), JumpObstacleFSMState);

	UBFsm::FStateCallback FallFSMState;
	FallFSMState.EnterDel.BindUObject(this, &ABLeon::FallEnter);
	FallFSMState.UpdateDel.BindUObject(this, &ABLeon::FallUpdate);
	FallFSMState.ExitDel.BindUObject(this, &ABLeon::FallExit);
	FsmComp->CreateState(TO_KEY(ELeonState::Fall), FallFSMState);

	UBFsm::FStateCallback ParryFSMState;
	ParryFSMState.EnterDel.BindUObject(this, &ABLeon::ParryEnter);
	ParryFSMState.UpdateDel.BindUObject(this, &ABLeon::ParryUpdate);
	ParryFSMState.ExitDel.BindUObject(this, &ABLeon::ParryExit);
	FsmComp->CreateState(TO_KEY(ELeonState::Parry), ParryFSMState);

	UBFsm::FStateCallback OpenGateFSMState;
	OpenGateFSMState.EnterDel.BindUObject(this, &ABLeon::OpenGateEnter);
	OpenGateFSMState.UpdateDel.BindUObject(this, &ABLeon::OpenGateUpdate);
	OpenGateFSMState.ExitDel.BindUObject(this, &ABLeon::OpenGateExit);
	FsmComp->CreateState(TO_KEY(ELeonState::OpenGate), OpenGateFSMState);

	UBFsm::FStateCallback OpenDoorFSMState;
	OpenDoorFSMState.EnterDel.BindUObject(this, &ABLeon::OpenDoorEnter);
	OpenDoorFSMState.UpdateDel.BindUObject(this, &ABLeon::OpenDoorUpdate);
	OpenDoorFSMState.ExitDel.BindUObject(this, &ABLeon::OpenDoorExit);
	FsmComp->CreateState(TO_KEY(ELeonState::OpenDoor), OpenDoorFSMState);

	UBFsm::FStateCallback BreakBoxFSMState;
	BreakBoxFSMState.EnterDel.BindUObject(this, &ABLeon::BreakBoxEnter);
	BreakBoxFSMState.UpdateDel.BindUObject(this, &ABLeon::BreakBoxUpdate);
	BreakBoxFSMState.ExitDel.BindUObject(this, &ABLeon::BreakBoxExit);
	FsmComp->CreateState(TO_KEY(ELeonState::BreakBox), BreakBoxFSMState);

	UBFsm::FStateCallback CutSceneFSMState;
	CutSceneFSMState.EnterDel.BindUObject(this, &ABLeon::CutsceenEnter);
	CutSceneFSMState.ExitDel.BindUObject(this, &ABLeon::CutsceenExit);
	FsmComp->CreateState(TO_KEY(ELeonState::CutScene00), CutSceneFSMState);
}

void ABLeon::CreateCollision()
{
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;

	InteractionObserver = GetWorld()->SpawnActor<ABCollisionObserverSphere>(SpawnParams);
	InteractionObserver->AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform);
	InteractionObserver->SetCollisionProfileName("Interaction");
	InteractionObserver->SetActorRelativeLocation(FVector(0, 0, 30));
	InteractionObserver->SetRadius(120.0f);

	KickOverlapObserver = GetWorld()->SpawnActor<ABCollisionObserverCapsule>(SpawnParams);
	KickOverlapObserver->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, "R_ShinSocket");
	KickOverlapObserver->SetActorRelativeLocation(FVector(1.3, 6.45, 0));
	KickOverlapObserver->SetActorRelativeRotation(FRotator(0.0, -9.5, 81.5));
	KickOverlapObserver->SetRadius(10.0f);
	KickOverlapObserver->SetHeight(30.0f);
	KickOverlapObserver->CollisionEnterCallback.BindUObject(this, &ABLeon::KickAttack);
	KickOverlapObserver->SetVisibilityCollision(false);
}

bool ABLeon::AbleWeaponSwap() const
{
	return true;
}

void ABLeon::UseQuickSlot(const uint32 _Index)
{
	if (false == AbleWeaponSwap())
	{
		return;
	}

	if (_Index > 7)
	{
		LOG_ERROR(TEXT("Wrong QuickSlot Index"));
		return;
	}

	LOG_MSG(TEXT("Swap Slot : %d"), _Index);
	HUD->SetQuickSlotNumber(_Index);

	ABInventoryWeapon* QuickSlotWeaponClass = InventoryActor->GetQuickSlot(_Index);

	if (nullptr == QuickSlotWeaponClass)
	{
		return;
	}

	EItemCode QuickSlotItemCode = QuickSlotWeaponClass->GetItemCode();

	if (EItemCode::Empty == QuickSlotItemCode)
	{
		return;
	}

	ChangeUseWeapon(QuickSlotItemCode);
}

ABLeonWeapon* ABLeon::CreateWeapon(EItemCode _WeaponCode)
{
	ABLeonWeapon* NewWeapon = nullptr;

	FName Path;
	LerpSocketStart = TEXT("");

	AActor* NewWeaponActor = nullptr;

	// Todo : 런타임 로딩이 아닌 생성자에서 로딩하도록 변경
	switch (_WeaponCode)
	{
	case EItemCode::Handgun_SR09R: // Pistol
		NewWeaponActor = GetWorld()->SpawnActor<AActor>(PistolActor);
		LerpSocketEnd = TEXT("R_PistolSocket");
		bIsLerpWeaponChange = false;
		break;
	case EItemCode::Shotgun_W870: // Shotgun
		NewWeaponActor = GetWorld()->SpawnActor<AActor>(ShotgunActor);
		LerpSocketStart = TEXT("ShotgunSpineSocket");
		LerpSocketEnd = TEXT("R_ShotgunSocket");
		bIsLerpWeaponChange = true;

		SocketLocationBlend = AbsolutAlphaBlend;
		SocketRotationBlend = PutOutTurnAlphaBlend;

		SocketLocationLerpSpeed = 10.0f;
		SocketRotationLerpSpeed = 2.0f;

		SocketLocationBlend = PutAwayLocationAlphaBlend;
		SocketRotationBlend = AbsolutAlphaBlend;
		SocketLocationBlend.SetBlendTime(1.0f / SocketLocationLerpSpeed);
		SocketRotationBlend.SetBlendTime(1.0f / SocketRotationLerpSpeed);
		SocketLocationBlend.Reset();
		SocketRotationBlend.Reset();

		SocketLocationLerpTime = 0.0f;
		SocketRotationLerpTime = 0.0f;
		break;
	case EItemCode::Rifle_SRM1903: // Rifle
		NewWeaponActor = GetWorld()->SpawnActor<AActor>(RifleActor);
		LerpSocketStart = TEXT("RifleSpineSocket");
		LerpSocketEnd = TEXT("R_RifleSocket");
		bIsLerpWeaponChange = true;

		SocketLocationBlend = AbsolutAlphaBlend;
		SocketRotationBlend = PutOutTurnAlphaBlend;

		SocketLocationLerpSpeed = 10.0f;
		SocketRotationLerpSpeed = 2.0f;

		SocketLocationBlend = PutAwayLocationAlphaBlend;
		SocketRotationBlend = AbsolutAlphaBlend;
		SocketLocationBlend.SetBlendTime(1.0f / SocketLocationLerpSpeed);
		SocketRotationBlend.SetBlendTime(1.0f / SocketRotationLerpSpeed);
		SocketLocationBlend.Reset();
		SocketRotationBlend.Reset();

		SocketLocationLerpTime = 0.0f;
		SocketRotationLerpTime = 0.0f;
		break;
	case EItemCode::CombatKnife: // Knife
		NewWeaponActor = GetWorld()->SpawnActor<AActor>(KnifeActor);
		LerpSocketEnd = TEXT("R_KnifeSocket");
		bIsLerpWeaponChange = false;
		break;
	case EItemCode::Grenade: // Grenade
		NewWeaponActor = GetWorld()->SpawnActor<AActor>(GrenadeActor);
		LerpSocketEnd = TEXT("R_GrenadeSocket");
		bIsLerpWeaponChange = false;
		break;
	case EItemCode::Flashbang: // Flashbang
		NewWeaponActor = GetWorld()->SpawnActor<AActor>(FlashbangActor);
		LerpSocketEnd = TEXT("R_FlashbangSocket");
		bIsLerpWeaponChange = false;
		break;
	default:
	{
		const UEnum* EnupPtr = FindObject<UEnum>(nullptr, TEXT("/Script/BiohazardRe4.EItemCode"), true);

		FName CodeName;

		if (nullptr != EnupPtr)
		{
			CodeName = EnupPtr->GetNameByValue((int64)_WeaponCode);
		}

		LOG_FATAL(TEXT("Wrong Item Code. Cannot be use as Weapons [%s]"), *CodeName.ToString());
	}
	break;
	}

	if (nullptr == NewWeaponActor)
	{
		LOG_FATAL(TEXT("Failed Create Weapon Actor"));
	}

	NewWeapon = Cast<ABLeonWeapon>(NewWeaponActor);

	if (nullptr == NewWeapon)
	{
		LOG_FATAL(TEXT("Failed Cast Weapon Class"));
	}

	NewWeapon->SetPlayer(this);

	return NewWeapon;
}

void ABLeon::DeleteCurrentWeapon()
{
	if (nullptr != CurrentWeapon)
	{
		bool IsDestroy = CurrentWeapon->Destroy();

		if (false == IsDestroy)
		{
			LOG_FATAL(TEXT("Fail Weapon Destroy"));
		}
		else
		{
			CurrentWeapon = nullptr;
		}
	}
	HUD->ClearWeapon();
}

ELeonWeaponAnim ABLeon::GetUseWeaponAnimation(EItemCode _WeaponCode) const
{
	switch (_WeaponCode)
	{
	case EItemCode::Empty:
		return ELeonWeaponAnim::Empty;
	case EItemCode::Handgun_SR09R:
		return ELeonWeaponAnim::Pistol;
	case EItemCode::Shotgun_W870:
		return ELeonWeaponAnim::Shotgun;
	case EItemCode::Rifle_SRM1903:
		return ELeonWeaponAnim::Rifle;
	case EItemCode::CombatKnife:
		return ELeonWeaponAnim::Knife;
	case EItemCode::Grenade:
		return ELeonWeaponAnim::Grenade;
	case EItemCode::Flashbang:
		return ELeonWeaponAnim::Grenade;
	default:
	{
		const UEnum* EnupPtr = FindObject<UEnum>(nullptr, TEXT("/Script/BiohazardRe4.EItemCode"), true);

		FName CodeName;

		if (nullptr != EnupPtr)
		{
			CodeName = EnupPtr->GetNameByValue((int64)_WeaponCode);
		}

		LOG_FATAL(TEXT("Wrong Item Code. Cannot be use as Weapons [%s]"), *CodeName.ToString());
	}
	return ELeonWeaponAnim::Empty;
	}
}

double ABLeon::GetAxisZAngle(const FVector& _Location) const
{
	FVector ActorForward = GetActorForwardVector();
	ActorForward.Z = 0.0f;
	ActorForward.Normalize();

	LOG_MSG(TEXT("Forward X : %f, Y : %f, Z : %f"), ActorForward.X, ActorForward.Y, ActorForward.Z);

	FVector ToDirection = _Location - GetActorLocation();
	ToDirection.Z = 0.0f;
	ToDirection.Normalize();

	LOG_MSG(TEXT("ToDirection X : %f, Y : %f, Z : %f"), ToDirection.X, ToDirection.Y, ToDirection.Z);

	double Angle = FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(ActorForward, ToDirection)));


	FVector Cross = FVector::CrossProduct(ActorForward, ToDirection);

	LOG_MSG(TEXT("Cross : %f, Y : %f, Z : %f"), Cross.X, Cross.Y, Cross.Z);

	if (0 >= Cross.Z)
	{
		Angle *= -1.0f;
	}

	LOG_MSG(TEXT("Angle : %f"), Angle);

	return Angle;
}

void ABLeon::KickAttack(AActor* _OverlapActor)
{
	if (ELeonState::KickAttack != LeonFSMState && ELeonState::BreakBox != LeonFSMState)
	{
		return;
	}

	if (false == bIsKickAttackActive)
	{
		return;
	}

	UGameplayStatics::ApplyDamage(_OverlapActor, 180, GetController(), this, KickDamageType);
}

void ABLeon::CutsceenEnter()
{
	bIsJog = true;
	bIsMove = true;
	bIsCrouch = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;

	JogTurnAngle = 0.0;
}

void ABLeon::CutsceenExit()
{
	bIsJog = false;
	bIsMove = false;

	GetCharacterMovement()->bOrientRotationToMovement = false;
}
