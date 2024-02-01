// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Player/Body/BLeon.h"
#include "Generic/BFsm.h"
#include "InputAction.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Components/InputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "BiohazardRe4.h"
#include "../Weapon/Grenade/BLeonGrenade.h"
#include "../Weapon/Gun/BLeonPistol.h"
#include "../Weapon/Gun/BLeonRifle.h"
#include "../Weapon/Gun/BLeonShotgun.h"
#include "../Weapon/Knife/BLeonKnife.h"

const FVector ABLeon::StandSocketOffset = FVector(0.0, 50.0, 80.0);
const FVector ABLeon::CrouchSocketOffset = FVector(0.0, 50.0, 10.0);

// Sets default values
ABLeon::ABLeon()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CreateSprintArm();
	CreateFSM();
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
	FsmComp->ChangeState(TO_KEY(ELeonState::Idle));
}

// Called every frame
void ABLeon::Tick(float _DeltaTime)
{
	Super::Tick(_DeltaTime);
	SpringArmUpdate(_DeltaTime);
	UseWeaponUpdate(_DeltaTime);
	WeaponSocketUpdate(_DeltaTime);

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

	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());

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

	for (uint32 i = 0; i < 8; ++i)
	{
		if (nullptr == QuickSlotActions[i])
		{
			LOG_FATAL(TEXT("is Not Set QuickSlotActions[%d]"), i);
		}

		Input->BindAction(QuickSlotActions[i], ETriggerEvent::Started, this, &ABLeon::UseQuickSlot, i);
	}

	Input->BindAction(WeaponPutAwayAction, ETriggerEvent::Started, this, &ABLeon::ChangeUseWeapon, EItemCode::Empty);
	Input->BindAction(ShootAction, ETriggerEvent::Started, this, &ABLeon::Shoot);
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

FTransform ABLeon::GetWeaponLeftSocketTransform() const
{
	if (nullptr == CurrentWeapon)
	{
		return FTransform::Identity;
	}

	return CurrentWeapon->GetLeftHandSocketTransform();
}

void ABLeon::WeaponPutOutStart()
{
	LOG_MSG(TEXT("Leon Weapon PutOutStart"));

	if (nullptr != CurrentWeapon)
	{
		DeleteCurrentWeapon();
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

void ABLeon::AttachLeftHandSocket()
{
	if (nullptr == CurrentWeapon)
	{
		return;
	}

	FName Socket = NAME_None;
	CurrentWeapon->AttachToComponent(nullptr, FAttachmentTransformRules::KeepRelativeTransform, Socket);

	//switch (UseWeaponCode)
	//{
	//case EItemCode::Handgun_SR09R:
	//	Socket = "L_PistolSocket";
	//	break;
	//case EItemCode::Shotgun_W870:
	//	Socket = "L_ShotgunSocket";
	//	break;
	//case EItemCode::Rifle_SRM1903:
	//	Socket = "L_RifleSocket";
	//	break;
	//case EItemCode::CombatKnife:
	//	Socket = "L_KnifeSocket";
	//	break;
	//case EItemCode::Grenade:
	//	Socket = "L_GrenadeSocket";
	//	break;
	//case EItemCode::Flashbang:
	//	Socket = "L_FlashbangSocket";
	//	break;
	//}
	//
	//CurrentWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, Socket);
}

void ABLeon::AttachRightHandSocket()
{
	if (nullptr == CurrentWeapon)
	{
		return;
	}

	CurrentWeapon->AttachToComponent(nullptr, FAttachmentTransformRules::KeepRelativeTransform);
	FName Socket = NAME_None;

	switch (UseWeaponCode)
	{
	case EItemCode::Handgun_SR09R:
		Socket = "R_PistolSocket";
		break;
	case EItemCode::Shotgun_W870:
		Socket = "R_ShotgunSocket";
		break;
	case EItemCode::Rifle_SRM1903:
		Socket = "R_RifleSocket";
		break;
	case EItemCode::CombatKnife:
		Socket = "R_KnifeSocket";
		break;
	case EItemCode::Grenade:
		Socket = "R_GrenadeSocket";
		break;
	case EItemCode::Flashbang:
		Socket = "R_FlashbangSocket";
		break;
	}

	CurrentWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, Socket);
}

void ABLeon::ChangeUseWeapon(EItemCode _WeaponCode)
{
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

void ABLeon::PlayMove(const FInputActionInstance& _MoveAction)
{
	FVector2D MoveInput2D = _MoveAction.GetValue().Get<FVector2D>();
	MoveInput = FVector(MoveInput2D.X, MoveInput2D.Y, 0);
}

void ABLeon::PlayIdle(const FInputActionInstance& _MoveAction)
{
	MoveInput = FVector::ZeroVector;
}

void ABLeon::TryInteraction()
{
	// Todo :: 상호작용 처리
}

bool ABLeon::AbleAim() const
{
	if (LeonWeaponSwap != ELeonWeaponSwap::None)
	{
		return false;
	}

	if (LeonWeaponState == ELeonWeaponAnim::Empty)
	{
		return false;
	}

	return true;
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
	if (true == bIsCrouch)
	{
		SpringArm->SocketOffset = FMath::VInterpConstantTo(SpringArm->SocketOffset, CrouchSocketOffset, _DeltaTime, 200.0f);
	}
	else
	{
		SpringArm->SocketOffset = FMath::VInterpConstantTo(SpringArm->SocketOffset, StandSocketOffset, _DeltaTime, 200.0f);
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

void ABLeon::JogLookAt(float _DeltaTime)
{
	FVector JogInput = MoveDir;
	VPlayerCameraToWorld(JogInput);
	FRotator InputRotator = JogInput.Rotation();

	SetActorRotation(FMath::RInterpConstantTo(GetActorRotation(), InputRotator, _DeltaTime, 360.0f));
}

bool ABLeon::AbleShoot() const
{
	int32 FSMKey = FsmComp->GetCurrentFSMKey();
	ELeonState FSMState = static_cast<ELeonState>(FSMKey);

	if (FSMState != ELeonState::Aim)
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

	return true;
}

void ABLeon::Shoot()
{
	LOG_MSG(TEXT("Try Player Weapon Shoot"));
	
	if (true == AbleShoot())
	{
		bIsWeaponShootTrigger = true;
	}
}

void ABLeon::Aim(float _DeltaTime)
{
	FVector CameraForward = PlayerCamera->GetForwardVector();
	CameraForward.Z = 0.0;
	FRotator CameraRotator = CameraForward.Rotation();

	SetActorRotation(FMath::RInterpConstantTo(GetActorRotation(), CameraRotator, _DeltaTime, 360.0f));
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

	bIsCrouch = ~bIsCrouch;
}

void ABLeon::CreateSprintArm()
{
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
	SpringArm->PrimaryComponentTick.bCanEverTick = true;
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->TargetArmLength = 300.0f;
	SpringArm->bUsePawnControlRotation = true;
	SpringArm->bInheritPitch = true;
	SpringArm->bInheritYaw = true;
	SpringArm->bInheritRoll = true;
	SpringArm->bEnableCameraLag = true;
	SpringArm->CameraLagSpeed = 10.0f;
	SpringArm->bEnableCameraRotationLag = true;
	SpringArm->CameraRotationLagSpeed = 10.0f;
	SpringArm->SocketOffset = StandSocketOffset;

	bUseControllerRotationRoll = false;
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;

	PlayerCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Player Camera"));
	PlayerCamera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
}

void ABLeon::CreateFSM()
{
	FsmComp = CreateDefaultSubobject<UBFsm>(TEXT("FSM Component"));

	UBFsm::FStateCallback StandUpState;
	StandUpState.EnterDel.BindUObject(this, &ABLeon::IdleEnter);
	StandUpState.UpdateDel.BindUObject(this, &ABLeon::IdleUpdate);
	StandUpState.ExitDel.BindUObject(this, &ABLeon::IdleExit);
	FsmComp->CreateState(TO_KEY(ELeonState::Idle), StandUpState);

	UBFsm::FStateCallback WalkState;
	WalkState.EnterDel.BindUObject(this, &ABLeon::WalkEnter);
	WalkState.UpdateDel.BindUObject(this, &ABLeon::WalkUpdate);
	WalkState.ExitDel.BindUObject(this, &ABLeon::WalkExit);
	FsmComp->CreateState(TO_KEY(ELeonState::Walk), WalkState);

	UBFsm::FStateCallback JogMoveState;
	JogMoveState.EnterDel.BindUObject(this, &ABLeon::JogEnter);
	JogMoveState.UpdateDel.BindUObject(this, &ABLeon::JogUpdate);
	JogMoveState.ExitDel.BindUObject(this, &ABLeon::JogExit);
	FsmComp->CreateState(TO_KEY(ELeonState::Jog), JogMoveState);

	UBFsm::FStateCallback AimState;
	AimState.EnterDel.BindUObject(this, &ABLeon::AimEnter);
	AimState.UpdateDel.BindUObject(this, &ABLeon::AimUpdate);
	AimState.ExitDel.BindUObject(this, &ABLeon::AimExit);
	FsmComp->CreateState(TO_KEY(ELeonState::Aim), AimState);
}

void ABLeon::UseQuickSlot(const uint32 _Index)
{
	if (_Index > 7)
	{
		LOG_ERROR(TEXT("Wrong QuickSlot Index"));
		return;
	}

	LOG_MSG(TEXT("Swap Slot : %d"), _Index);

	// Todo : 디버그용 퀵슬롯 정보가 완성되면 교체
	switch (_Index)
	{
	case 0:
	{
		ChangeUseWeapon(EItemCode::Empty);
	}
		break;
	case 1:
	{
		ChangeUseWeapon(EItemCode::Handgun_SR09R);
	}
	break;
	case 2:
	{
		ChangeUseWeapon(EItemCode::Shotgun_W870);
	}
	break;
	case 3:
	{
		ChangeUseWeapon(EItemCode::Rifle_SRM1903);
	}
	break;
	case 4:
	{
		ChangeUseWeapon(EItemCode::Grenade);
	}
	break;
	case 5:
	{
		ChangeUseWeapon(EItemCode::Flashbang);
	}
	break;
	case 6:
	{
		ChangeUseWeapon(EItemCode::CombatKnife);
	}
	break;

	default:
		break;
	}

}

ABLeonWeapon* ABLeon::CreateWeapon(EItemCode _WeaponCode)
{
	ABLeonWeapon* NewWeapon = nullptr;

	FName Path;
	LerpSocketStart = TEXT("");

	// Todo : 런타임 로딩이 아닌 생성자에서 로딩하도록 변경
	switch (_WeaponCode)
	{
	case EItemCode::Handgun_SR09R: // Pistol
		Path = TEXT("/Script/Engine.Blueprint'/Game/Blueprints/Actor/Player/Weapon/Gun/BP_Leon_Pistol_SR09R.BP_Leon_Pistol_SR09R'");
		LerpSocketEnd = TEXT("R_PistolSocket");
		bIsLerpWeaponChange = false;
		break;
	case EItemCode::Shotgun_W870: // Shotgun
		Path = TEXT("/Script/Engine.Blueprint'/Game/Blueprints/Actor/Player/Weapon/Gun/BP_Leon_Shotgun_W870.BP_Leon_Shotgun_W870'");
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
		Path = TEXT("/Script/Engine.Blueprint'/Game/Blueprints/Actor/Player/Weapon/Gun/BP_Leon_Rifle_SRM1903.BP_Leon_Rifle_SRM1903'");
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
		Path = TEXT("/Script/Engine.Blueprint'/Game/Blueprints/Actor/Player/Weapon/Knife/BP_Leon_Knife_CombatKnife.BP_Leon_Knife_CombatKnife'");
		LerpSocketEnd = TEXT("R_KnifeSocket");
		bIsLerpWeaponChange = false;
		break;
	case EItemCode::Grenade: // Grenade
		Path = TEXT("/Script/Engine.Blueprint'/Game/Blueprints/Actor/Player/Weapon/Grenade/BP_Leon_Grenade_HandGrenade.BP_Leon_Grenade_HandGrenade'");
		LerpSocketEnd = TEXT("R_GrenadeSocket");
		bIsLerpWeaponChange = false;
		break;
	case EItemCode::Flashbang: // Flashbang
		Path = TEXT("/Script/Engine.Blueprint'/Game/Blueprints/Actor/Player/Weapon/Grenade/BP_Leon_Grenade_Flashbang.BP_Leon_Grenade_Flashbang'");
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

	UBlueprint* ObjectToSpawn = Cast<UBlueprint>(StaticLoadObject(UBlueprint::StaticClass(), NULL, *Path.ToString()));

	if (nullptr == ObjectToSpawn)
	{
		LOG_FATAL(TEXT("Failed Find Weapon Class"));
	}

	AActor* NewWeaponActor = GetWorld()->SpawnActor<AActor>(ObjectToSpawn->GeneratedClass);

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