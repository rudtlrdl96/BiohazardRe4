// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BMonsterBase.h"
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
	virtual void BeginPlay() override;

	virtual void SetlMeshAndAnimationByRandomInBeginPlay();

	virtual void SetClothesSkeletalMeshByRandomInBeginPlay();
	virtual void SetWeaponSkeletalMeshByRandomInBeginPlay();
	virtual void SetAnimInstanceAndAnimationMontageInBeginPlay();

	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	void InitAI();
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Mesh Components")
	TObjectPtr<class USkeletalMeshComponent> BodyBase = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
	EWeaponType MyWeaponType;

private:
	virtual void InitDamageTypes() override;
	virtual void SetDamagedSectionNums() override;

};
