// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actor/Monster/MonsterActor/BMonsterBase.h"
#include "BMonsterMale.generated.h"

/**
 * 
 */
UCLASS()
class BIOHAZARDRE4_API ABMonsterMale : public ABMonsterBase
{
	GENERATED_BODY()
	
public:
	ABMonsterMale();
	
protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Mesh Components")
	USkeletalMeshComponent* BodyBase = nullptr;
	
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

private:
	void CreateComponent();
	void InitAI();
	void InitValue();

	void SetSkeletalMeshByRandomInBeginPlay();
	void SetSkeletalMeshInConstructor();
};
