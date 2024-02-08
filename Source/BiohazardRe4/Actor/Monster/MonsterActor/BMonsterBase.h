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
	Normal,
	Point,
	Radial,
};

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

	virtual void DamagedEnd() override;
	virtual void SetCurrentState(EMonsterState _InState) override;
	virtual void Attack() override;
	virtual bool IsAttacking() override;
	virtual void SetIsAttack(bool _IsAttacking) override;
	virtual void SetMonsterAttackEndDelegate(FMonsterAttackEnd& _InAttackEnd) override;
	virtual const FMonsterAttackEnd& GetMonsterAttackEndDelegate() override;
	virtual void AttackStart() override;

	//IBMonsterStatInterface
public:
	virtual float GetRunSpeed() override;
	virtual float GetWalkSpeed() override;
	virtual float GetAttackRadius() override;
	virtual float GetDetectRadius() override;
	virtual float GetPatrolRadius() override;
	virtual float GetAttackSweepRadius() override;

	virtual void StatInit(const class UBMonsterStatData* _DataAsset);
	
	UFUNCTION(BlueprintCallable)
	virtual float GetWalkDistanceThreshold() override;

	//IBInteraction
public:
	virtual bool AbleInteraction() const override;
	virtual EInteraction GetInteractionType() const override;
	virtual FVector GetUIPivot() const override;

	//Actor Function
public:
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	virtual void BeginPlay() override;

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

	//Delegate
protected:
	FMonsterAttackEnd OnAttackEnd;

	//Variable
protected:
	TArray<class UClass*> DamageTypes;
	TMap<FString, TMap<FString, int>> DamagedMontageSectionNums;

	//Init
protected:
	virtual void InitDamageTypes();
	virtual void SetDamagedSectionNums();

	//Damaged
private:
	float TakeNormalDamage(const FDamageEvent& _DamageEvent, const AActor* DamageCauser, float _DamagedAmount);
	float TakePointDamage(const FDamageEvent& _DamageEvent, float _DamagedAmount);
	void TakeRadialDamage();
	
	void DamagedByGun(const FString& _DamagedPart);
	void DamagedByKnife(const FString& _DamagedPart);

	void DamagedByKick(const FDamageEvent& _DamageEvent, const AActor* DamageCauser);

	void SmallDamaged(const FString& _DamagedPart);
	void MediumDamaged(const FString& _DamagedPart);
	void LargeDamaged(const FString& _DamagedPart);

	const FString GetDamagedPartToString(const FPointDamageEvent* const& _DamageEvent);
	float CaculatePointDamage(float _OriginDamage, const FString& _DamagedPart, EPlayerDamageType _PlayerDamageType);
	float CaculateNormalDamage(float _OriginDamage, EPlayerDamageType _PlayerDamageType);
	float CaculateCriticalDamage(float _OriginDamage, EPlayerDamageType _PlayerDamageType);

	virtual void MonsterDeath(EDeathType _DeathType, const FDamageEvent& _DamageEvent);
	virtual void Flashed();


	//State Variable
private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = State, Meta = (AllowPrivateAccess = "true"))
	EMonsterState CurState;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = State, Meta = (AllowPrivateAccess = "true"))
	uint8 bIsAttacking : 1;

	//Interaction Variable
private:
	EInteraction InteractionType;

};
