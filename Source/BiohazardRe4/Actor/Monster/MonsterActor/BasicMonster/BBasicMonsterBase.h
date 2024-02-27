// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actor/Monster/MonsterActor/BMonsterBase.h"
#include "BBasicMonsterBase.generated.h"

UENUM()
enum class EMeshType
{
	Weapon,
	Hat,
	Head,
	Jacket,
	Pants,
};

USTRUCT()
struct FMontageStruct
{
	GENERATED_BODY()

	FMontageStruct()
	{

	}

	FMontageStruct(TObjectPtr<class UAnimMontage> _AttackMontage, TObjectPtr<class UAnimMontage> _DamagedMontage, TObjectPtr<class UAnimMontage> _ParriedMontage)
		:AttackMontage(_AttackMontage), DamagedMontage(_DamagedMontage), ParriedMontage(_ParriedMontage)
	{

	}

	TObjectPtr<class UAnimMontage> AttackMontage;
	TObjectPtr<class UAnimMontage> DamagedMontage;
	TObjectPtr<class UAnimMontage> ParriedMontage;
};

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

	UFUNCTION(BlueprintCallable)
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
	void AnimInstanceLoad();
	void MontageLoad();

protected:
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Mesh Components")
	TObjectPtr<class USkeletalMeshComponent> BodyBase = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AIMode")
	EAIMode AIMode;

protected:
	static TArray<TPair<FString, TObjectPtr<class USkeletalMesh>>> LoadedWeaponMesh;
	static TSubclassOf<class UAnimInstance> LoadedAnimInstance;
	static TMap<EWeaponType, FMontageStruct> LoadedMontage;

private:
	virtual void InitDamageTypes() override;
	virtual void SetDamagedSectionNums() override;

	//class ABAIBasicMonsterController* MyAIController = nullptr;
};
