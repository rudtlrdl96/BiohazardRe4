// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actor/Monster/MonsterActor/BasicMonster/BBasicMonsterBase.h"
#include "BMonsterMale.generated.h"

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
	virtual void Tick(float _DeltaTIme) override;

protected:
	virtual void SetClothesSkeletalMeshByRandomInBeginPlay() override;
	virtual void SetAnimInstanceAndAnimationMontageInBeginPlay() override;
	virtual void AllCollisionOff() override;

private:
	void CreateComponent();

	void SetSkeletalMeshInConstructor();
	void InitSoundCues();

	void MeshLoad();

	void AttachDamagedCollisionComponentToMesh();
	void CreateDamagedCollisionComponent();

private:
	static TMap<EMeshType, TArray<TObjectPtr<class USkeletalMesh>>> LoadedMesh;
	TMap<FString, TPair<FString, TObjectPtr<class UCapsuleComponent>>> DamagedCollisions;
};
