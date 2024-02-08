// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Actor/Generic/Interface/BInteraction.h"
#include "BJumpObstacleTrigger.generated.h"

class UBoxComponent;

USTRUCT()
struct FJumpData
{
public:
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere)
	uint32 bAbleJump = false;
	UPROPERTY(VisibleAnywhere)
	FVector Start = FVector::ZeroVector;
	UPROPERTY(VisibleAnywhere)
	FVector End = FVector::ZeroVector;
	UPROPERTY(VisibleAnywhere)
	FVector MoveVector = FVector::ForwardVector;
};

UCLASS()
class BIOHAZARDRE4_API ABJumpObstacleTrigger : public AActor, public IBInteraction
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABJumpObstacleTrigger();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	UPROPERTY(EditAnywhere, Category = "Jump")
	UBoxComponent* LocationATrigger = nullptr;

	UPROPERTY(EditAnywhere, Category = "Jump")
	UBoxComponent* LocationBTrigger = nullptr;

	// 현재 상호작용이 가능한 상태인지
	virtual bool AbleInteraction() const override
	{
		return false == bIsUse;
	}

	// 현재 상호작용의 상태를 반환
	virtual EInteraction GetInteractionType() const
	{
		return EInteraction::JumpObstacle;
	}

	// 변수 기준 가까운 트리거 위치를 반환합니다
	virtual FJumpData GetJumpMetaData(const FVector& _Location) const;

	inline bool UseWindow()
	{
		if (false == bIsUse)
		{
			bIsUse = true;
			return true;
		}

		return false;
	}

	inline void UseEnd()
	{
		bIsUse = false;
	}

protected:
	uint8 bIsUse = false;
};
