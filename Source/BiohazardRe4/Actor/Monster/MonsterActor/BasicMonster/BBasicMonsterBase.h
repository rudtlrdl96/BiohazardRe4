// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actor/Monster/MonsterActor/BMonsterBase.h"
#include "BBasicMonsterBase.generated.h"

enum class EWeaponName : uint8
{
	HandAxe,
	KitchenKnife,
	Sickle,
	Torch,
	Hands,
};

UENUM(BlueprintType)
enum class EAIMode : uint8
{
	Idle UMETA(DisplayName = "Idle"),
	Patrol UMETA(DisplayName = "Patrol"),
};

UCLASS()
class BIOHAZARDRE4_API ABBasicMonsterBase : public ABMonsterBase
{
	GENERATED_BODY()

public:
	ABBasicMonsterBase();
	void SetAIMode(EAIMode _AIMode);
	
	UFUNCTION(BlueprintCallable)
	void AIOff();
	UFUNCTION(BlueprintCallable)
	void AIOn();

protected:
	virtual void BeginPlay() override;
	void InitAI();

	//Init Mesh, Animation
protected:
	//Top
	virtual void SetlMeshAndAnimationByRandomInBeginPlay();

	//Inner
	virtual void SetWeaponSkeletalMeshByRandomInBeginPlay();
	virtual void SetClothesSkeletalMeshByRandomInBeginPlay();
	virtual void SetAnimInstanceAndAnimationMontageInBeginPlay();

	virtual void SetWeaponCollision(EWeaponName _WeaponName);
	void LoadWeaponMesh();

protected:
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Mesh Components")
	TObjectPtr<class USkeletalMeshComponent> BodyBase = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AIMode")
	EAIMode AIMode;

private:
	virtual void InitDamageTypes() override;
	virtual void SetDamagedSectionNums() override;

	static TArray<TPair<FString, TObjectPtr<class USkeletalMesh>>> LoadedWeaponMesh;
	//class ABAIBasicMonsterController* MyAIController = nullptr;
};
