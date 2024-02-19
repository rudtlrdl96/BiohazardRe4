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

	UFUNCTION(BlueprintCallable)
	void AIStart();
protected:
	virtual void BeginPlay() override;

private:
	virtual void MonsterDeathByPoint(const FDamageEvent& _DamageEvent);
	virtual void MonsterDeathByNormal(const FDamageEvent& _DamageEvent, const AActor* DamageCauser);

	virtual void InitDamageTypes() override;
	virtual void SetDamagedSectionNums() override;
};
