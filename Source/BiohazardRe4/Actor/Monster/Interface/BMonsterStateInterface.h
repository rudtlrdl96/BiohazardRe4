// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "BMonsterStateInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UBMonsterStateInterface : public UInterface
{
	GENERATED_BODY()
};

DECLARE_DELEGATE(FMonsterAttackEnd);

UENUM(BlueprintType)
enum class EMonsterState : uint8
{
	Idle UMETA(DisplayName = "Idle"),
	Turn UMETA(DisplayName = "Turn"),
	Patrol UMETA(DisplayName = "Patrol"),
	Walk UMETA(DisplayName = "Walk"),
	Run UMETA(DisplayName = "Run"),
	Attack UMETA(DisplayName = "Attack"),
	Flashed UMETA(DisplayName = "Flashed"),
	Damaged UMETA(DisplayName = "Damaged"),
	Kicked UMETA(DisplayName = "Kicked"),
	Death UMETA(DisplayName = "Death"),
	Groggy UMETA(DisplayName = "Groggy"),
};

class BIOHAZARDRE4_API IBMonsterStateInterface
{
	GENERATED_BODY()

public:
	virtual void Attack() = 0;
	virtual void AttackStart() = 0;
	virtual void DamagedEnd() = 0;

	virtual void Parry() = 0;
	virtual void ParryTimeOn() = 0;
	virtual void ParryTimeOff() = 0;
	virtual bool isAbleParring() = 0;

	virtual void GroggyEnd() = 0;

	virtual EMonsterState GetCurrentState() = 0;
	virtual void SetCurrentState(EMonsterState _InState) = 0;
	virtual const FMonsterAttackEnd& GetMonsterAttackEndDelegate() = 0;
	virtual void SetMonsterAttackEndDelegate(FMonsterAttackEnd& _InAttackEnd) = 0;

protected:

private:
};
