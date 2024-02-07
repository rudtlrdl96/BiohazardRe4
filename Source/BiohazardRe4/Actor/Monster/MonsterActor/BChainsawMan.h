// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actor/Monster/MonsterActor/BMonsterBase.h"
#include "BChainsawMan.generated.h"
/**
 * 
 */
UCLASS()
class BIOHAZARDRE4_API ABChainsawMan : public ABMonsterBase
{
	GENERATED_BODY()

public:
	ABChainsawMan();

protected:
	virtual void Attack() override;
	virtual void BeginPlay() override;

private:

	virtual void MonsterDeath(EDeathType _DeathType, const FPointDamageEvent* const& _DamageEvent);
	virtual void InitDamageTypes() override;
	virtual void SetDamagedSectionNums() override;
};
