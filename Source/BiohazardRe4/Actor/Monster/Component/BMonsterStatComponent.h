// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BMonsterStatComponent.generated.h"

USTRUCT()
struct FStatStruct
{
	GENERATED_BODY()

	float MaxHp = 0.0f;
	float CurrentHp = 0.0f;

	float DetectRadius = 0.0f;
	float PatrolRadius = 0.0f;

	float RunSpeed = 0.0f;
	float WalkSpeed = 0.0f;
	float WalkDistanceThreshold = 0.0f;

	float BaseAttackPower = 0.0f;
	float GroggyThreshold = 0.0f;

	float AttackRange = 0.0f;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BIOHAZARDRE4_API UBMonsterStatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UBMonsterStatComponent();

	float GetRunSpeed() const { return RunSpeed; };
	float GetWalkSpeed() const { return WalkSPeed; };
	float GetCurrentHp() const { return CurrentHp; };
	float GetAttackRange() const { return AttackRange; };
	float GetDetectRadius() const { return DetectRadius; };
	float GetPatrolRadius() const { return PatrolRadius; };
	float GetBaseAttackPower() const { return BaseAttackPower; };
	float GetGroggyThreshold() const { return GroggyThreshold; };
	float GetWalkDistanceThreshold() const { return WalkDistanceThreshold; };
	void StatInit(const FStatStruct& _StatData);

	void SetHp(float _Hp) { CurrentHp = _Hp; };
	void DecreaseHp(float _Hp);
	
	UFUNCTION(BlueprintCallable)
	bool isDeath() const { return CurrentHp <= 0; };

protected:
	virtual void BeginPlay() override;

protected:
	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat)
	float MaxHp = 0.0f;

	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat)
	float CurrentHp = 0.0f;

	UPROPERTY(VisibleAnywhere, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	float DetectRadius = 0.0f;

	UPROPERTY(VisibleAnywhere, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	float PatrolRadius = 0.0f;

	UPROPERTY(VisibleAnywhere, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	float RunSpeed = 0.0f;

	UPROPERTY(VisibleAnywhere, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	float WalkSPeed = 0.0f;
	
	UPROPERTY(VisibleAnywhere, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	float WalkDistanceThreshold = 0.0f;

	UPROPERTY(VisibleAnywhere, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	float BaseAttackPower = 0.0f;

	UPROPERTY(VisibleAnywhere, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	float GroggyThreshold = 0.0f;

	UPROPERTY(VisibleAnywhere, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	float AttackRange = 0.0f;
};
