// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Actor/Monster/Interface/BMonsterAnimInterface.h"
#include "BMonsterAnimInstanceBase.generated.h"

/**
 * 
 */
UCLASS()
class BIOHAZARDRE4_API UBMonsterAnimInstanceBase : public UAnimInstance, public IBMonsterAnimInterface
{
	GENERATED_BODY()
public:
	UBMonsterAnimInstanceBase();

	virtual void SetTarget(UObject* _Target) override;
	virtual void SetAnimationType(EMonsterAnimType _AnimType) override;
	virtual void SetDamagedBlendAlpha(float _Alpha) override;
protected:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float _DeltaSeconds) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Character)
	TObjectPtr<class ACharacter> Owner;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Character)
	TObjectPtr<class UCharacterMovementComponent> Movement;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = State)
	uint8 CurState;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = State)
	EMonsterAnimType AnimType;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Character)
	float GroundSpeed;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Character)
	float DistanceToPlayer;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Character)
	float WalkDistanceThreshold;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Character)
	float DamagedBlendAlpha;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Character)
	FRotator GroundRotation;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Character)
	UObject* TraceTarget;
private:
	float GroundSpeedThreshold = 0.0f;

	EMonsterAnimType SavedAnimType;
};
