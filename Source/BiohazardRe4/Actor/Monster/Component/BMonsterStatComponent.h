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
	float AttackRadius = 0.0f;

	float AttackSweepRadius = 0.0f;
	float PatrolRadius = 0.0f;

	float RunSpeed = 0.0f;
	float WalkSpeed = 0.0f;
	float WalkDistanceThreshold = 0.0f;

	float BaseAttackPower = 0.0f;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BIOHAZARDRE4_API UBMonsterStatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UBMonsterStatComponent();

	void SetAttackRadius(float _AttackRadius) { AttackRadius = _AttackRadius; };
	float GetAttackRadius() { return AttackRadius; };
	float GetAttackSweepRadius() { return AttackSweepRadius; };
	float GetDetectRadius() { return DetectRadius; };
	float GetPatrolRadius() { return PatrolRadius; };
	float GetWalkSpeed() { return WalkSPeed; };
	float GetRunSpeed() { return RunSpeed; };
	float GetWalkDistanceThreshold() { return WalkDistanceThreshold; };
	float GetBaseAttackPower() { return BaseAttackPower; };

	void StatInit(const FStatStruct& _StatData);

	void SetHp(float _Hp) { CurrentHp = _Hp; };
	void DecreaseHp(float _Hp);
	
	bool isDeath() { return CurrentHp <= 0; };

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
	float AttackRadius = 0.0f;

	UPROPERTY(VisibleAnywhere, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	float AttackSweepRadius = 0.0f;

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

};
