// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BMonsterBase.generated.h"

UCLASS()
class BIOHAZARDRE4_API ABMonsterBase : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABMonsterBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

};
