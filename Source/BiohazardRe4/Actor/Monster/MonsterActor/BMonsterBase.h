// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "DamageType/BDMGPlayerDamage.h"
#include "DamageType/BDMGMonsterDamage.h"
#include "Actor/Generic/Interface/BInteraction.h"
#include "Actor/Monster/Interface/BMonsterStatInterface.h"
#include "Actor/Monster/Interface/BMonsterStateInterface.h"
#include "Actor/Monster/Interface/BMonsterSoundInterface.h"
#include "BMonsterBase.generated.h"

UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	None UMETA(DisplayName = "None"),
	OneHand UMETA(DisplayName = "OneHand"),
	TwoHands UMETA(DisplayName = "TwoHands"),
};

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
	Kick,
	Knife,
	Grenade,
};

DECLARE_DELEGATE(FOnLandedByBurstJump)
DECLARE_DELEGATE(FOnLandedByCrossWindowJump)

UCLASS()
class BIOHAZARDRE4_API ABMonsterBase : public ACharacter, public IBMonsterStateInterface, public IBMonsterSoundInterface, public IBMonsterStatInterface, public IBInteraction
{
	GENERATED_BODY()

	//Constructor
public:
	ABMonsterBase();
	
	//IBMonsterStateInterface
public:
	UFUNCTION(BlueprintCallable)
	virtual EMonsterState GetCurrentState() override;

	UFUNCTION(BlueprintCallable)
	virtual void CrossWindowStart() override;
	virtual void CrossWindowJumpStart() override;

	virtual void AttackStart() override;
	virtual void AttackOn() override;
	virtual void AttackOff() override;

	virtual bool isDamaged() override;
	virtual void DamagedEnd() override;
	virtual void GroggyEnd() override;
	
	virtual void Parry() override;
	virtual void ParryTimeOn() override;
	virtual void ParryTimeOff() override;
	virtual bool isAbleParring() override;

	virtual void SetCurrentState(EMonsterState _InState) override;

	virtual const FMonsterAttackEnd& GetMonsterAttackEndDelegate() override;
	virtual void SetMonsterAttackEndDelegate(FMonsterAttackEnd& _InAttackEnd) override;

	//IBMonsterStatInterface
public:
	virtual float GetRunSpeed() const override;
	virtual float GetWalkSpeed() const override;
	virtual float GetAttackRange() const override;
	virtual float GetDetectRadius() const override;
	virtual float GetPatrolRadius() const override;
	virtual float GetGroggyThreshold() const override;
	virtual void StatInit(const class UBMonsterStatData* _DataAsset);
	
	UFUNCTION(BlueprintCallable)
	virtual float GetWalkDistanceThreshold() const override;

	//IBMonsterSoundInteface
public:
	virtual void PlaySound(ESoundType _PlaySoundType) override;
	virtual bool isAblePlay(ESoundType _PlaySoundType) override;
	virtual void PlayFootSound(EFootSoundType _PlaySoundType, bool _isLeft) override;

	//IBInteraction
public:
	virtual bool AbleInteraction() const override;
	virtual EInteraction GetInteractionType() const override;
	virtual FVector GetUIPivot() const override;

	UFUNCTION(BlueprintImplementableEvent)
	void OnParryInteraction();
	UFUNCTION(BlueprintImplementableEvent)
	void OffParryInteraction();

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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Equipment, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UCapsuleComponent> WeaponCollision = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Audio, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAudioComponent> GeneralSoundComp = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Audio, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAudioComponent> LeftFootSoundComp = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Audio, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAudioComponent> RightFootSoundComp = nullptr;

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
	FOnLandedByCrossWindowJump OnLandedByCrossWindowJump;

	//Variable
protected:
	TArray<class UClass*> DamageTypes;
	TMap<FString, TMap<FString, int>> DamagedMontageSectionNums;

	TMap<ESoundType, TObjectPtr<class USoundCue>> GeneralSoundCues;
	TMap<EFootSoundType, TObjectPtr<class USoundCue>> FootSoundCues;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
	EWeaponType MyWeaponType;

	//Init
protected:
	virtual void InitDamageTypes();
	virtual void SetDamagedSectionNums();
	virtual void InitSoundCues();
	void LoadFootSoundCue();

	//Damaged
protected:
	float TakePointDamage(const FDamageEvent& _DamageEvent, const AActor* DamageCauser, float _DamagedAmount);
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


	virtual void MonsterDeath(EDeathType _DeathType, const FDamageEvent& _DamageEvent, const AActor* DamageCauser);
	virtual void MonsterDeathByKick(const FDamageEvent& _DamageEvent, const AActor* DamageCauser);
	virtual void MonsterDeathByPoint(const FDamageEvent& _DamageEvent);
	virtual void MonsterDeathByKnife(const FDamageEvent& _DamageEvent, const AActor* DamageCauser);
	virtual void MonsterDeathByGrenade(const FDamageEvent& _DamageEvent, const AActor* DamageCauser);

	virtual void AllCollisionOff();
	//virtual void MonsterDeathByRadial(EDeathType _DeathType, const FDamageEvent& _DamageEvent);

	void DamagedByFlashed();
	void BurstJumpUpdate();
	void AttackUpdate();

	void CrossWindow();
	void CrossWindowJumpUpdate();
	void CrossWindowJumpEnd();

	void CreateBloodParticle(FVector _Location, FRotator _Rotator);

	bool isSetTargetInBlackBoard();

	UFUNCTION(BlueprintCallable)
	void SetTeleport(class ANavLinkProxy* _Proxy);

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
	uint8 bIsAttacking : 1;
	uint8 bIsKnifeDamaged : 1;

	FTimerHandle TimerHandle;
	FTimerHandle KnifeDamagedTimerHandle;
	FTimerHandle CollisionOffTimer;

	float GroggyAmount = 0.0f;

	//particle
private:
	UParticleSystem* BloodParticle;
};
