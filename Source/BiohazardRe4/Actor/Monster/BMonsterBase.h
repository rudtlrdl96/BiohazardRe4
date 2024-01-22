// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interface/BMonsterStateInterface.h"
#include "Interface/BMonsterStatInterface.h"
#include "BMonsterBase.generated.h"

UCLASS()
class BIOHAZARDRE4_API ABMonsterBase : public ACharacter, public IBMonsterStateInterface, public IBMonsterStatInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABMonsterBase();
	
	UFUNCTION(BlueprintCallable)
	virtual MonsterState GetCurrentState() override;
	virtual void SetCurrentState(MonsterState _InState) override;
	virtual void Attack() override;
	virtual bool IsAttacking() override;
	virtual void SetIsAttack(bool _IsAttacking) override;
	virtual void SetMonsterAttackEndDelegate(FMonsterAttackEnd& _InAttackEnd) override;
	virtual const FMonsterAttackEnd& GetMonsterAttackEndDelegate() override;
	virtual float GetAttackRange() override;
	virtual float GetWeaponAttackRadius() override;
	virtual float GetDetectRange() override;
	virtual float GetPatrolRadius() override;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UBMonsterStatComponent> Stat;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Equipment, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USkeletalMeshComponent> Weapon;

	FMonsterAttackEnd OnAttackEnd;
private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = State, Meta = (AllowPrivateAccess = "true"))
	MonsterState CurState;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = State, Meta = (AllowPrivateAccess = "true"))
	uint8 bIsAttacking : 1;


};
