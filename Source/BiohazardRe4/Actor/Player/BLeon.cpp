// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Player/BLeon.h"
#include "Generic/BFsm.h"
#include "InputAction.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Components/InputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

const FVector ABLeon::StandSocketOffset = FVector(0.0, 50.0, 80.0);
const FVector ABLeon::CrouchSocketOffset = FVector(0.0, 50.0, 0.0);


// Sets default values
ABLeon::ABLeon()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
	SpringArm->PrimaryComponentTick.bCanEverTick = true;
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->TargetArmLength = 120.0f;
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

	UCameraComponent* Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Player Camera"));
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);

	FsmComp = CreateDefaultSubobject<UBFsm>(TEXT("FSM Component"));

	UBFsm::FStateCallback StandUpState;
	StandUpState.EnterDel.BindUObject(this, &ABLeon::StandUpEnter);
	StandUpState.UpdateDel.BindUObject(this, &ABLeon::StandUpUpdate);
	StandUpState.ExitDel.BindUObject(this, &ABLeon::StandUpExit);
	FsmComp->CreateState(TO_KEY(ELeonState::StandUP), StandUpState);

	UBFsm::FStateCallback WalkState;
	WalkState.EnterDel.BindUObject(this, &ABLeon::WalkEnter);
	WalkState.UpdateDel.BindUObject(this, &ABLeon::WalkUpdate);
	WalkState.ExitDel.BindUObject(this, &ABLeon::WalkExit);
	FsmComp->CreateState(TO_KEY(ELeonState::Walk), WalkState);

	UBFsm::FStateCallback JogState;
	JogState.EnterDel.BindUObject(this, &ABLeon::JogEnter);
	JogState.UpdateDel.BindUObject(this, &ABLeon::JogUpdate);
	JogState.ExitDel.BindUObject(this, &ABLeon::JogExit);
	FsmComp->CreateState(TO_KEY(ELeonState::Jog), JogState);

	UBFsm::FStateCallback CrouchState;
	CrouchState.EnterDel.BindUObject(this, &ABLeon::CrouchEnter);
	CrouchState.UpdateDel.BindUObject(this, &ABLeon::CrouchUpdate);
	CrouchState.ExitDel.BindUObject(this, &ABLeon::CrouchExit);
	FsmComp->CreateState(TO_KEY(ELeonState::Crouch), CrouchState);

	//USkeletalMeshComponent* BodyMesh = GetMesh();
}

// Called when the game starts or when spawned
void ABLeon::BeginPlay()
{
	Super::BeginPlay();

	FsmComp->ChangeState(TO_KEY(ELeonState::StandUP));
}

// Called every frame
void ABLeon::Tick(float _DeltaTime)
{
	Super::Tick(_DeltaTime);

	if (true == bIsCrouch)
	{
		SpringArm->SocketOffset = FMath::VInterpConstantTo(SpringArm->SocketOffset, CrouchSocketOffset, _DeltaTime, 140.0f);
	}
	else
	{
		SpringArm->SocketOffset = FMath::VInterpConstantTo(SpringArm->SocketOffset, StandSocketOffset, _DeltaTime, 140.0f);
	}
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
		Input->BindAction(LookAction, ETriggerEvent::Triggered, this, &ABLeon::PlayLook);

		Input->BindAction(JogAction, ETriggerEvent::Completed, this, &ABLeon::TryJog);
		Input->BindAction(CrouchAction, ETriggerEvent::Completed, this, &ABLeon::TryCrouch);
		Input->BindAction(InteractionActor, ETriggerEvent::Completed, this, &ABLeon::PlayInteraction);
	}
}

void ABLeon::PlayMove(const FInputActionInstance& _MoveAction)
{
	MoveDir = _MoveAction.GetValue().Get<FVector2D>();

	if (true == bIsJog)
	{
		PlayJog();
	}
	else if (true == bIsCrouch)
	{
		PlayCrouch();
	}
	else
	{
		PlayWalk();
	}
}

void ABLeon::PlayWalk()
{
}

void ABLeon::PlayJog()
{
}

void ABLeon::PlayCrouch()
{
}

void ABLeon::PlayInteraction()
{

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

void ABLeon::TryJog()
{
	// Able Jog Check

	bIsJog = true;
}

void ABLeon::TryCrouch()
{
	// Able Crouch Check
	bIsCrouch = ~bIsCrouch;
}