// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "../Interface/BMonsterStateInterface.h"
#include "../Interface/BMonsterStatInterface.h"
#include "DamageType/BDMGPlayerDamage.h"
#include "DamageType/BDMGMonsterDamage.h"
#include "Actor/Generic/Interface/BInteraction.h"
#include "BMonsterBase.generated.h"

UENUM()
enum class EDamagedPart
{
	None,
	Head,
	LArm,
	RArm,
	LLEg,
	RLEg,
	Body,
};

UENUM()
enum class EDeathType
{
	Point,
	Radial,
};

UCLASS()
class BIOHAZARDRE4_API ABMonsterBase : public ACharacter, public IBMonsterStateInterface, public IBMonsterStatInterface, public IBInteraction
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABMonsterBase();
	
	//State
public:
	UFUNCTION(BlueprintCallable)
	virtual EMonsterState GetCurrentState() override;

	virtual void DamagedEnd() override;
	virtual void SetCurrentState(EMonsterState _InState) override;
	virtual void Attack() override;
	virtual bool IsAttacking() override;
	virtual void SetIsAttack(bool _IsAttacking) override;
	virtual void SetMonsterAttackEndDelegate(FMonsterAttackEnd& _InAttackEnd) override;
	virtual const FMonsterAttackEnd& GetMonsterAttackEndDelegate() override;

	//Stat
public:
	virtual float GetAttackRadius() override;
	virtual float GetAttackSweepRadius() override;
	virtual float GetDetectRadius() override;
	virtual float GetPatrolRadius() override;
	virtual float GetRunSpeed() override;
	virtual float GetWalkSpeed() override;

	UFUNCTION(BlueprintCallable)
	virtual float GetWalkDistanceThreshold() override;

	virtual void StatInit(const class UBMonsterStatData* _DataAsset);
public:
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
protected:
	virtual void BeginPlay() override;
	virtual void AttackStart() override;

	virtual void InitDamageTypes();
	//Component
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UBMonsterStatComponent> Stat = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Equipment, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USkeletalMeshComponent> Weapon = nullptr;

	//Montage
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	TObjectPtr<class UAnimMontage> AttackMontage = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	TObjectPtr<class UAnimMontage> DamagedMontage = nullptr;

protected:
	FMonsterAttackEnd OnAttackEnd;
	TArray<class UClass*> DamageTypes;
	TMap<FString, TMap<FString, int>> DamagedMontageSectionNums;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = State, Meta = (AllowPrivateAccess = "true"))
	EMonsterState CurState;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = State, Meta = (AllowPrivateAccess = "true"))
	uint8 bIsAttacking : 1;

private:
	virtual void SetDamagedSectionNums();

	float TakePointDamage(const FPointDamageEvent* const& _DamageEvent, float _DamagedAmount);
	void TakeRadialDamage();

	const FString GetDamagedPartToString(const FPointDamageEvent* const& _DamageEvent);
	
	void DamagedByGun(const FString& _DamagedPart);
	void DamagedByKnife(const FString& _DamagedPart);
	void DamagedByKick(const FString& _DamagedPart);

	void SmallDamaged(const FString& _DamagedPart);
	void MediumDamaged(const FString& _DamagedPart);
	void LargeDamaged(const FString& _DamagedPart);

	float CaculateDamage(float _OriginDamage, const FString& _DamagedPart, EPlayerDamageType _PlayerDamageType);

	virtual void MonsterDeath(EDeathType _DeathType, const FPointDamageEvent* const& _DamageEvent);
	virtual void Flashed();

public:
	virtual bool AbleInteraction() const override;

	// 현재 상호작용의 상태를 반환
	virtual EInteraction GetInteractionType() const override;

	// UI Pivot 위치를 반환
	virtual FVector GetUIPivot() const override;

	uint8 bisAbleInteraction : 1;
	EInteraction InteractionType;
};
