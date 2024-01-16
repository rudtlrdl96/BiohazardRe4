// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Player/BLeon.h"
#include "Generic/BFsm.h"

// Sets default values
ABLeon::ABLeon()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

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
	JogState.EnterDel.BindUObject(this, &ABLeon::StandUpEnter);
	JogState.UpdateDel.BindUObject(this, &ABLeon::StandUpUpdate);
	JogState.ExitDel.BindUObject(this, &ABLeon::StandUpExit);
	FsmComp->CreateState(TO_KEY(ELeonState::Jog), JogState);

	UBFsm::FStateCallback CrouchState;
	CrouchState.EnterDel.BindUObject(this, &ABLeon::StandUpEnter);
	CrouchState.UpdateDel.BindUObject(this, &ABLeon::StandUpUpdate);
	CrouchState.ExitDel.BindUObject(this, &ABLeon::StandUpExit);
	FsmComp->CreateState(TO_KEY(ELeonState::Crouch), CrouchState);
}

// Called when the game starts or when spawned
void ABLeon::BeginPlay()
{
	Super::BeginPlay();

	FsmComp->ChangeState(TO_KEY(ELeonState::StandUP));
}

void ABLeon::PlayWalk(const FInputActionInstance& _MoveAction)
{

}

void ABLeon::PlayJog(const FVector& _MoveDirection)
{

}

void ABLeon::PlayCrouch(const FVector& _MoveDirection)
{

}

void ABLeon::PlayInteraction()
{

}

void ABLeon::TryJog()
{
	// Able Jog Check

	bIsJog = true;
}

void ABLeon::TryCrouch()
{
	// Able Crouch Check

	bIsCrouch = true;
}

// Called every frame
void ABLeon::Tick(float _DeltaTime)
{
	Super::Tick(_DeltaTime);

}

// Called to bind functionality to input
void ABLeon::SetupPlayerInputComponent(UInputComponent* _PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(_PlayerInputComponent);

}

