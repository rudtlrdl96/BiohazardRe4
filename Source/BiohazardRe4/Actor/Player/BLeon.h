// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BLeon.generated.h"
  
struct FInputActionInstance;
class UBFsm;
class UInputMappingContext;
class UInputAction;
class USpringArmComponent;
class UCameraComponent;

#define TO_KEY(EnumValue) static_cast<int32>(EnumValue)

UCLASS()
class BIOHAZARDRE4_API ABLeon : public ACharacter
{
	GENERATED_BODY()

private:
	enum class ELeonState
	{
		Idle	= 0,
		Walk	= 1,
		Jog		= 2,
	};

	static const FVector StandSocketOffset;
	static const FVector CrouchSocketOffset;

public:
	// Sets default values for this character's properties
	ABLeon();

	// Called every frame
	virtual void Tick(float _DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* _PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable)
	bool IsCrouch() const
	{
		return bIsCrouch;
	}

	UFUNCTION(BlueprintCallable)
	FVector GetMoveDirection() const
	{
		return MoveDir;
	}

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere, Category = Animation)
	uint32 bIsMove : 1 = false;

	UPROPERTY(VisibleAnywhere, Category = Animation)
	uint32 bIsJog : 1 = false;

	UPROPERTY(VisibleAnywhere, Category = Animation)
	uint32 bIsCrouch : 1 = false;

	UPROPERTY(EditAnywhere, Category = Input)
	float TurnSpeed = 20.0f;

	UPROPERTY(EditAnywhere, Category = Input)
	UInputMappingContext* DefaultMappingContext = nullptr;
		
	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* MoveAction = nullptr;

	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* LookAction = nullptr;

	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* JogAction = nullptr;

	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* CrouchAction = nullptr;

	UPROPERTY(EditAnywhere, Category = Input)

	UInputAction* InteractionActor = nullptr;

	USpringArmComponent* SpringArm = nullptr;
	UCameraComponent* PlayerCamera = nullptr;

	FVector MoveDir = FVector::ZeroVector;

	UBFsm* FsmComp = nullptr;

	void PlayMove(const FInputActionInstance& _MoveAction);
	void PlayIdle(const FInputActionInstance& _MoveAction);
	void PlayWalk();
	void PlayJog();
	void PlayCrouch();
	void PlayInteraction();

	virtual void PlayLook(const FInputActionInstance& _LookAction);
	virtual void PlayLook(const FVector2D& _LookAction);

	void TryJog();
	void TryCrouch();

	/* FSM */
	void IdleEnter();
	void IdleUpdate(float _DeltaTime);
	void IdleExit();

	void WalkEnter();
	void WalkUpdate(float _DeltaTime);
	void WalkExit();

	void JogEnter();
	void JogUpdate(float _DeltaTime);
	void JogExit();
};
