// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interface/BMonsterStateInterface.h"
#include "BMonsterBase.generated.h"

UCLASS()
class BIOHAZARDRE4_API ABMonsterBase : public ACharacter, public IBMonsterStateInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABMonsterBase();

	virtual MonsterState GetCurrentState() override;
	virtual void SetCurrentState(MonsterState _InState) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = State, Meta = (AllowPrivateAccess = "true"))
	MonsterState CurState;
};
