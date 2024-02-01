// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actor/Monster/Animation/AnimInstance/BMonsterAnimInstanceBase.h"
#include "BBasicMonsterAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class BIOHAZARDRE4_API UBBasicMonsterAnimInstance : public UBMonsterAnimInstanceBase
{
	GENERATED_BODY()
	
protected:
	virtual void NativeUpdateAnimation(float _DeltaSeconds) override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = State)
	uint8 CurWeaponType;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Character)
	UObject* TraceTarget;
};
