// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BMapBGMBase.generated.h"

UCLASS()
class BIOHAZARDRE4_API ABMapBGMBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABMapBGMBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(BlueprintReadWrite,EditAnywhere, Category = Sound)
	TObjectPtr<class UAudioComponent> Sound_IdleBGM = nullptr;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Sound)
	TObjectPtr<class UAudioComponent> Sound_SubIdleBGM = nullptr;
	UPROPERTY(BlueprintReadWrite, EditAnywhere,Category = Sound)
	TObjectPtr<class UAudioComponent> Sound_HeightenBGM= nullptr;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Sound)
	TObjectPtr<class UAudioComponent> Sound_ChainsawBGM = nullptr;

private:
	UFUNCTION(BlueprintCallable)
	void PlayIdleBGM();

	UFUNCTION(BlueprintCallable)
	void PlayHeightenSound();

	void SetSoundCue();
};
