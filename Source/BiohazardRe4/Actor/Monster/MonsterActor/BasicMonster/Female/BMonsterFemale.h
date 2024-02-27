// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actor/Monster/MonsterActor/BasicMonster/BBasicMonsterBase.h"
#include "BMonsterFemale.generated.h"

/**
 * 
 */
UCLASS()
class BIOHAZARDRE4_API ABMonsterFemale : public ABBasicMonsterBase
{
	GENERATED_BODY()
public:
	ABMonsterFemale();

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
	
private:
	void CreateComponent();
	void MeshLoad();
	void CreateDamagedCollisionComponent();
	void AttachDamagedCollisionComponentToMesh();
	void SetClothesSkeletalMeshByRandomInBeginPlay() override;
	virtual void SetAnimInstanceAndAnimationMontageInBeginPlay() override;

private:
	static TMap<EMeshType, TArray<TObjectPtr<class USkeletalMesh>>> LoadedMesh;

	TMap<FString, TPair<FString, TObjectPtr<class UCapsuleComponent>>> DamagedCollisions;
};
