// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actor/Monster/MonsterActor/BasicMonster/BBasicMonsterBase.h"
#include "BMonsterMale.generated.h"

UENUM()
enum class EMeshType
{
	Weapon,
	Hat,
	Head,
	Jacket,
	Pants,
};

UENUM()
enum class EMeshAnimType
{
	Base,
	Copy,
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

UCLASS()
class BIOHAZARDRE4_API ABMonsterMale : public ABBasicMonsterBase
{
	GENERATED_BODY()
	
public:
	ABMonsterMale();
	
protected:
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Mesh Components")
	USkeletalMeshComponent* Head = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Mesh Components")
	USkeletalMeshComponent* Jacket = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Mesh Components")
	USkeletalMeshComponent* Pants = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Mesh Components")
	USkeletalMeshComponent* Hat = nullptr;

	UFUNCTION()
	virtual void BeginPlay() override;

protected:
	virtual void SetClothesSkeletalMeshByRandomInBeginPlay() override;
	virtual void SetAnimInstanceAndAnimationMontageInBeginPlay() override;
	virtual void AllCollisionOff() override;

private:
	void CreateComponent();
	void InitValue();

	void SetSkeletalMeshInConstructor();
	void InitSoundCues();

	void MeshLoad();
	void MontageLoad();
	void AnimInstanceLoad();

private:
	static TMap<EMeshType, TArray<TObjectPtr<class USkeletalMesh>>> LoadedMesh;
	static TMap<EMeshAnimType, TSubclassOf<class UAnimInstance>> LoadedAnimInstance;
	static TMap<EWeaponType, FMontageStruct> LoadedMontage;
	
	virtual void Tick(float _DeltaTIme) override;
};
