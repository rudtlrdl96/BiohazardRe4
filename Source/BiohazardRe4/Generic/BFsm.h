// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include <functional>
#include "BFsm.generated.h"

DECLARE_DELEGATE(EnterCallback)
DECLARE_DELEGATE_OneParam(UpdateCallback, float)
DECLARE_DELEGATE(ExitCallback)

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BIOHAZARDRE4_API UBFsm : public UActorComponent
{
	GENERATED_BODY()
public:
	class FStateCallback
	{
	public:
		EnterCallback EnterDel;
		UpdateCallback UpdateDel;
		ExitCallback ExitDel;
	};

public:	
	// Sets default values for this component's properties
	UBFsm();

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void CreateState(int32 _Key, FStateCallback& _NewState);
	void DestroyState(int32 _Key);
	
	void ChangeState(int32 _Key, bool _bIsPlayEnterDel = true, bool _bIsPlayExitDel = true, bool _bIsForce = true);
	
	void PlayFSM(bool _bIsPlayEnterDel = true);
	void StopFSM(bool _bIsPlayExitDel = true);

	inline int32 GetCurrentFSMKey()
	{
		return FSMKey;
	}

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:	
	TMap<int32, FStateCallback> FsmStates;

	int32 FSMKey = INT32_MIN;
	uint32 bIsPlay : 1 = false;
};
