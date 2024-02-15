// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "DamageType/BDMGPlayerDamage.h"
#include "DamageType/BDMGMonsterDamage.h"
#include "Actor/Generic/Interface/BInteraction.h"
#include "Actor/Monster/Interface/BMonsterStatInterface.h"
#include "Actor/Monster/Interface/BMonsterStateInterface.h"
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

DECLARE_DELEGATE(FOnLandedByBurstJump)

UCLASS()
class BIOHAZARDRE4_API ABMonsterBase : public ACharacter, public IBMonsterStateInterface, public IBMonsterStatInterface, public IBInteraction
{
	GENERATED_BODY()

	//Constructor
public:
	ABMonsterBase();
	
	//IBMonsterStateInterface
public:
	UFUNCTION(BlueprintCallable)
	virtual EMonsterState GetCurrentState() override;

	virtual void Attack() override;
	virtual void AttackStart() override;
	
	virtual bool isDamaged() override;
	virtual void DamagedEnd() override;
	
	virtual void Parry() override;
	virtual void ParryTimeOn() override;
	virtual void ParryTimeOff() override;
	virtual bool isAbleParring() override;

	virtual void GroggyEnd() override;

	virtual void SetCurrentState(EMonsterState _InState) override;

	virtual const FMonsterAttackEnd& GetMonsterAttackEndDelegate() override;
	virtual void SetMonsterAttackEndDelegate(FMonsterAttackEnd& _InAttackEnd) override;

	//IBMonsterStatInterface
public:
	virtual float GetRunSpeed() const override;
	virtual float GetWalkSpeed() const override;
	virtual float GetAttackRadius() const override;
	virtual float GetDetectRadius() const override;
	virtual float GetPatrolRadius() const override;
	virtual float GetGroggyThreshold() const override;
	virtual float GetAttackSweepRadius() const override;
	virtual void StatInit(const class UBMonsterStatData* _DataAsset);
	
	UFUNCTION(BlueprintCallable)
	virtual float GetWalkDistanceThreshold() const override;

	//IBInteraction
public:
	virtual bool AbleInteraction() const override;
	virtual EInteraction GetInteractionType() const override;
	virtual FVector GetUIPivot() const override;

	//Actor Function
public:
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	virtual void BeginPlay() override;
	virtual void Tick(float _DeltaTime) override;
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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	TObjectPtr<class UAnimMontage> ParriedMontage = nullptr;

	//Delegate
protected:
	FMonsterAttackEnd OnAttackEnd;
	FOnLandedByBurstJump OnLandedByBurstJump;

	//Variable
protected:
	TArray<class UClass*> DamageTypes;
	TMap<FString, TMap<FString, int>> DamagedMontageSectionNums;

	//Init
protected:
	virtual void InitDamageTypes();
	virtual void SetDamagedSectionNums();

	//Damaged
protected:
	float TakePointDamage(const FDamageEvent& _DamageEvent, float _DamagedAmount);
	float TakeNormalDamage(const FDamageEvent& _DamageEvent, const AActor* DamageCauser, float _DamagedAmount);
	float TakeRadialDamage(const FDamageEvent& _DamageEvent, const AActor* DamageCauser, float _DamagedAmount);
	
	void DamagedByGun(const FString& _DamagedPart);
	void DamagedByKick(const FDamageEvent& _DamageEvent, const AActor* DamageCauser);
	void DamagedByKnife(const FDamageEvent& _DamageEvent);
	void DamagedByGrenade(const FDamageEvent& _DamageEvent, float _DamagedAmount);

	void BurstJumpStart(FVector _Direction, float _JumpPower, TFunction<void()> _OnLanded, bool _XYOverride, bool _ZOverride);
	const FString GetBurstJumpSectionName(FVector _MyLocation, FVector _JumpCauserLocation);

	void SmallDamaged(const FString& _DamagedPart);
	void MediumDamaged(const FString& _DamagedPart);
	void LargeDamaged(const FString& _DamagedPart);

	float CaculatePointDamage(float _OriginDamage, const FString& _DamagedPart, const EPlayerDamageType _PlayerDamageType);
	float CaculateNormalDamage(float _OriginDamage, const EPlayerDamageType _PlayerDamageType);
	float CaculateCriticalDamage(float _OriginDamage, const EPlayerDamageType _PlayerDamageType);
	
	const FString GetDamagedPartToString(const FPointDamageEvent* const& _DamageEvent);

	virtual void MonsterDeathByKick(const FDamageEvent& _DamageEvent, const AActor* DamageCauser);
	virtual void MonsterDeathByPoint(const FDamageEvent& _DamageEvent);
	virtual void MonsterDeathByKnife(const FDamageEvent& _DamageEvent, const AActor* DamageCauser);
	virtual void MonsterDeathByGrenade(const FDamageEvent& _DamageEvent, const AActor* DamageCauser);

	//virtual void MonsterDeathByRadial(EDeathType _DeathType, const FDamageEvent& _DamageEvent);

	virtual void DamagedByFlashed();
	void BurstJumpUpdate();

	//State Variable
private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = State, Meta = (AllowPrivateAccess = "true"))
	EMonsterState CurState;

	//Interaction Variable
private:
	uint8 bIsAbleParry : 1;
	EInteraction InteractionType;

	//Basic Variable
private:
	uint8 bIsDamaged : 1;
	uint8 bIsDamagedCooltime : 1;

	FTimerHandle TimerHandle;
	float GroggyAmount = 0.0f;
};
