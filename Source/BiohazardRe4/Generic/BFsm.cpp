// Fill out your copyright notice in the Description page of Project Settings.


#include "BFsm.h"

// Sets default values for this component's properties
UBFsm::UBFsm()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}

// Called when the game starts
void UBFsm::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void UBFsm::TickComponent(float _DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(_DeltaTime, TickType, ThisTickFunction);

	if (false == bIsPlay)
	{
		return;
	}
	
	if (false == FsmStates.Contains(FSMKey))
	{
		return;
	}
	
	FStateCallback& FindState = FsmStates[FSMKey];

	if (true == FindState.UpdateDel.IsBound())
	{
		FindState.UpdateDel.Execute(_DeltaTime);
	}
}


void UBFsm::CreateState(int32 _Key, FStateCallback& _NewState)
{
	if (true == FsmStates.Contains(_Key))
	{
		UE_LOG(LogTemp, Display, TEXT("FSM State Create Error : Create a duplicate State"));
		return;
	}

	FsmStates.Emplace(_Key, _NewState);
}

void UBFsm::DestroyState(int32 _Key)
{
	if (false == FsmStates.Contains(_Key))
	{
		// Todo : Log추가
		return;
	}

	FsmStates.Remove(_Key);
}

void UBFsm::ChangeState(int32 _Key, bool _bIsPlayEnterDel /*= true*/, bool _bIsPlayExitDel /*= true*/)
{
	bIsPlay = true;

	if (true == _bIsPlayExitDel && true == FsmStates.Contains(FSMKey))
	{
		FStateCallback& FindState = FsmStates[FSMKey];

		if (true == FindState.ExitDel.IsBound())
		{
			FindState.ExitDel.Execute();
		}
	}

	if (false == FsmStates.Contains(_Key))
	{
		// Tood : Error Log 추가

		StopFSM(false);
		FSMKey = INT32_MIN;
		return;
	}

	FSMKey = _Key;

	if (true == _bIsPlayEnterDel)
	{
		FStateCallback& FindState = FsmStates[FSMKey];

		if (true == FindState.EnterDel.IsBound())
		{
			FindState.EnterDel.Execute();
		}
	}
}

void UBFsm::PlayFSM(bool _bIsPlayEnterDel)
{
	bIsPlay = true;

	if (false == FsmStates.Contains(FSMKey))
	{
		return;
	}

	if (true == _bIsPlayEnterDel)
	{
		FStateCallback& FindState = FsmStates[FSMKey];

		if (true == FindState.EnterDel.IsBound())
		{
			FindState.EnterDel.Execute();
		}
	}
}

void UBFsm::StopFSM(bool _bIsPlayExitDel)
{
	bIsPlay = false;

	if (false == FsmStates.Contains(FSMKey))
	{
		return;
	}

	if (true == _bIsPlayExitDel)
	{
		FStateCallback& FindState = FsmStates[FSMKey];

		if (true == FindState.EnterDel.IsBound())
		{
			FindState.EnterDel.Execute();
		}
	}
}
