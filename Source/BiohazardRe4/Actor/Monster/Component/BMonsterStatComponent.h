// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BMonsterStatComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BIOHAZARDRE4_API UBMonsterStatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UBMonsterStatComponent();

	float GetAttackRange() { return AttackRange; };
	float GetWeaponAttackRadius() { return WeaponAttackRadius; };
	float GetDetectRange() { return DetectRange; };
	float GetPatrolRadius() { return PatrolRadius; };
protected:
	virtual void BeginPlay() override;

protected:
	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat)
	float MaxHp = 0.0f;

	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat)
	float CurrentHp = 0.0f;

	UPROPERTY(VisibleInstanceOnly, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	float DetectRange = 0.0f;

	UPROPERTY(VisibleInstanceOnly, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	float AttackRange = 0.0f;

	UPROPERTY(VisibleInstanceOnly, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	float WeaponAttackRadius = 0.0f;

	UPROPERTY(VisibleInstanceOnly, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	float PatrolRadius = 0.0f;

};
