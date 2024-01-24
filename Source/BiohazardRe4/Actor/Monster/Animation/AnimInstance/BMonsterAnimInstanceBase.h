// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "../../Interface/BMonsterStateInterface.h"
#include "BMonsterAnimInstanceBase.generated.h"

/**
 * 
 */
UCLASS()
class BIOHAZARDRE4_API UBMonsterAnimInstanceBase : public UAnimInstance
{
	GENERATED_BODY()
public:
	UBMonsterAnimInstanceBase();

protected:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float _DeltaSeconds) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Character)
	TObjectPtr<class ACharacter> Owner;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Character)
	TObjectPtr<class UCharacterMovementComponent> Movement;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = State)
	uint8 CurState;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Character)
	float GroundSpeed;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Character)
	FRotator GroundRotation;
private:
	float GroundSpeedThreshold = 0.0f;
};
