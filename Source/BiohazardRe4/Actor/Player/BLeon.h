// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BLeon.generated.h"
  
struct FInputActionInstance;
class UBFsm;

#define TO_KEY(EnumValue) static_cast<int32>(EnumValue)

UCLASS()
class BIOHAZARDRE4_API ABLeon : public ACharacter
{
	GENERATED_BODY()

private:
	enum class ELeonState
	{
		StandUP = 0,
		Walk	= 1,
		Jog		= 2,
		Crouch	= 3,
	};

public:
	// Sets default values for this character's properties
	ABLeon();

	// Called every frame
	virtual void Tick(float _DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* _PlayerInputComponent) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	void PlayWalk(const FInputActionInstance& _MoveAction);
	void PlayJog(const FVector& _MoveDirection);
	void PlayCrouch(const FVector& _MoveDirection);
	void PlayInteraction();

	void TryJog();
	void TryCrouch();

	/* FSM */
	void StandUpEnter();
	void StandUpUpdate(float _DeltaTime);
	void StandUpExit();

	void WalkEnter();
	void WalkUpdate(float _DeltaTime);
	void WalkExit();



	uint32 bIsJog : 1 = false;
	uint32 bIsCrouch : 1 = false;

	UBFsm* FsmComp = nullptr;

};
