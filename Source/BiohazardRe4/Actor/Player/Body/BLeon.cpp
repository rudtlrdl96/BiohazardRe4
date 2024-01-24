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
}

void ABLeon::ChangeUseWeapon(EItemCode _WeaponCode)
{
	if (_WeaponCode == UseWeaponCode)
	{
		return;
	}

	switch (LeonWeaponSwap)
	{
	case ELeonWeaponSwap::PutAway:
	{
		WeaponPutOutAnim = GetUseWeaponAnimation(_WeaponCode);
	}
		break;
	default:
	{
		WeaponPutAwayAnim = GetUseWeaponAnimation(UseWeaponCode);
		WeaponPutOutAnim = GetUseWeaponAnimation(_WeaponCode);

		if (WeaponPutAwayAnim == ELeonWeaponAnim::Empty)
		{
			LeonWeaponSwap = ELeonWeaponSwap::PutOut;
		}
		else
		{
			LeonWeaponSwap = ELeonWeaponSwap::PutAway;
		}
	}
		break;
	}

	UseWeaponCode = _WeaponCode;
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
	switch (LeonWeaponSwap)
	{
	case ELeonWeaponSwap::None:
	{
		LeonWeaponState = WeaponPutOutAnim;
	}
		break;
	case ELeonWeaponSwap::PutOut:
	{	
		LeonWeaponState = WeaponPutOutAnim;
	}
		break;
	case ELeonWeaponSwap::PutAway:
	{
		LeonWeaponState = WeaponPutAwayAnim;
	}		
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

void ABLeon::JogLookAt(float _DeltaTime)
{
	FVector JogInput = MoveDir;
	VPlayerCameraToWorld(JogInput);
	FRotator InputRotator = JogInput.Rotation();

	SetActorRotation(FMath::RInterpConstantTo(GetActorRotation(), InputRotator, _DeltaTime, 360.0f));
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

	if (FSMState == ELeonState::Jog)
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
		ChangeUseWeapon(EItemCode::CombatKnife);
	}
	break;
	case 5:
	{
		ChangeUseWeapon(EItemCode::Grenade);
	}
	break;
	case 6:
	{
		ChangeUseWeapon(EItemCode::Flashbang);
	}
	break;

	default:
		break;
	}

}

ELeonWeaponAnim ABLeon::GetUseWeaponAnimation(EItemCode _WeaponCode)
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
		LOG_ERROR(TEXT("Wrong Use Weapon Code"));
	}
		return ELeonWeaponAnim::Empty;
	}
}
