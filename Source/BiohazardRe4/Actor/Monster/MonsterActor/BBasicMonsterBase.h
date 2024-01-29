// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actor/Monster/MonsterActor/BMonsterBase.h"
#include "BBasicMonsterBase.generated.h"

UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	None UMETA(DisplayName = "None"),
	OneHand UMETA(DisplayName = "OneHand"),
	TwoHands UMETA(DisplayName = "TwoHands"),
};

UCLASS()
class BIOHAZARDRE4_API ABBasicMonsterBase : public ABMonsterBase
{
	GENERATED_BODY()

public:
	ABBasicMonsterBase();

protected:
	void SetWeaponSkeletalMeshByRandomInBeginPlay();
	void InitAI();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Mesh Components")
	TObjectPtr<class USkeletalMeshComponent> BodyBase = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
	EWeaponType MyWeaponType;
};
