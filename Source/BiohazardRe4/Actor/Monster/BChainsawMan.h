// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actor/Monster/BMonsterBase.h"
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

	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Equipment, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USkeletalMeshComponent> Weapon;
};
