// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Player/BLeon.h"
#include "Generic/BFsm.h"
#include "InputAction.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Components/InputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

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
}

// Called to bind functionality to input
void ABLeon::SetupPlayerInputComponent(UInputComponent* _PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(_PlayerInputComponent);

	UEnhancedInputComponent* Input = Cast<UEnhancedInputComponent>(_PlayerInputComponent);

	APlayerController* PlayerController = Cast<APlayerController>(Controller);

	if (PlayerController != nullptr)
	{
		UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());

		if (Subsystem != nullptr)
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}

	// Failed Find InputComponent
	check(nullptr != Input);
	// is Not Set MoveAction
	check(nullptr != MoveAction);
	// is Not Set LookAction
	check(nullptr != LookAction);
	// is Not Set JogAction
	check(nullptr != JogAction);
	// is Not Set RunAction
	check(nullptr != CrouchAction);
	// is Not Set GunFireAction
	check(nullptr != InteractionActor);

	if (Input != nullptr)
	{
		Input->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ABLeon::PlayMove);
		Input->BindAction(MoveAction, ETriggerEvent::None, this, &ABLeon::PlayIdle);
		Input->BindAction(LookAction, ETriggerEvent::Triggered, this, &ABLeon::PlayLook);

		Input->BindAction(JogAction, ETriggerEvent::Triggered, this, &ABLeon::ActiveJog);
		Input->BindAction(JogAction, ETriggerEvent::Completed, this, &ABLeon::DisableJog);
		Input->BindAction(CrouchAction, ETriggerEvent::Completed, this, &ABLeon::TryCrouch);
		Input->BindAction(InteractionActor, ETriggerEvent::Completed, this, &ABLeon::TryInteraction);
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

void ABLeon::PlayLook(const FInputActionInstance& _LookAction)
{
	PlayLook(_LookAction.GetValue().Get<FVector2D>());
}

void ABLeon::PlayLook(const FVector2D& _LookAction)
{
	if (Controller != nullptr)
	{
		AddControllerYawInput(_LookAction.X * TurnSpeed * GetWorld()->GetDeltaSeconds());
		AddControllerPitchInput(_LookAction.Y * TurnSpeed * GetWorld()->GetDeltaSeconds());
	}
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

void ABLeon::GetJogInputForward(FVector& _Result) const
{
	FVector CameraLook = PlayerCamera->GetForwardVector();
	CameraLook.Z = 0.0;
	CameraLook = CameraLook.GetSafeNormal();

	_Result = MoveInput;
	_Result.X = -_Result.X;

	double CameraAngle = FMath::Atan2(CameraLook.X, CameraLook.Y);
	_Result = _Result.RotateAngleAxisRad(CameraAngle, FVector::DownVector);
}

double ABLeon::JogInputAngle() const
{
	FVector InputForward = FVector::ZeroVector;
	GetJogInputForward(InputForward);

	FVector PlayerForward = GetActorForwardVector();

	return FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(PlayerForward, InputForward)));
}

void ABLeon::JogLookAt(float _DeltaTime)
{
	FVector JogInput = FVector::ZeroVector;
	GetJogInputForward(JogInput);
	FRotator InputRotator = JogInput.Rotation();

	SetActorRotation(FMath::RInterpConstantTo(GetActorRotation(), InputRotator, _DeltaTime, 360.0f));
}

void ABLeon::ActiveJog()
{
	// Able Jog Check

	bIsJogTrigger = true;
}

void ABLeon::DisableJog()
{
	bIsJogTrigger = false;
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

	// Able Crouch Check
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

}