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
enum class MonsterState : uint8
{
	Idle UMETA(DisplayName = "Idle"),
	Patrol UMETA(DisplayName = "Patrol"),
	Walk UMETA(DisplayName = "Walk"),
	Run UMETA(DisplayName = "Run"),
	Attack UMETA(DisplayName = "Attack"),
};

class BIOHAZARDRE4_API IBMonsterStateInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual MonsterState GetCurrentState() = 0;
	virtual void SetCurrentState(MonsterState _InState) = 0;
	virtual bool IsAttacking() = 0;
	virtual void SetIsAttack(bool _IsAttacking) = 0;
	virtual void SetMonsterAttackEndDelegate(FMonsterAttackEnd& _InAttackEnd) = 0;
	virtual const FMonsterAttackEnd& GetMonsterAttackEndDelegate() = 0;
protected:

private:
};
