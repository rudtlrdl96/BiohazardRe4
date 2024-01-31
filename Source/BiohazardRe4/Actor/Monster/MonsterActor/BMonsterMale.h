// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actor/Monster/MonsterActor/BBasicMonsterBase.h"
#include "BMonsterMale.generated.h"

/**
 * 
 */
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

private:
	void CreateComponent();
	void InitValue();

	void SetSkeletalMeshInConstructor();

};
