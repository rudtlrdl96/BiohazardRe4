// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "BPistolAnimInstance.generated.h"


UENUM(BluePrintType)
enum class EPistolState : uint8
{
	Idle_Loop UMETA(DisplayName = "1"),
	NoAmmo_Loop UMETA(DisplayName = "2"),
	Ammo_Fire_Ammo UMETA(DisplayName = "3"),
	Ammo_Fire_NoAmmo UMETA(DisplayName = "4"),
	NoAmmo_Fire_NoAmmo UMETA(DisplayName = "5"),
	Ammo_Reload UMETA(DisplayName = "6"),
	NoAmmo_Reload UMETA(DisplayName = "7"),
	Putout UMETA(DisplayName = "8"),
};

UCLASS()
class BIOHAZARDRE4_API UBPistolAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:

	UPROPERTY()
	EPistolState State = EPistolState::Idle_Loop;

	UPROPERTY()
	class ABLeonPistol* PistolPtr;

};
