// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/StaticMeshActor.h"
#include "BEmptyShell.generated.h"

/**
 * 
 */
UCLASS()
class BIOHAZARDRE4_API ABEmptyShell : public AStaticMeshActor
{
	GENERATED_BODY()
	
public:

	ABEmptyShell();

	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, Category = "Option")
	float LifeTime = 3.f;
	
	UPROPERTY(VisibleAnywhere, Category = "Option")
	float Time = 0.f;

	UPROPERTY(EditDefaultsOnly, Category = "Sound")
	USoundBase* Sound_HitGround = nullptr;
	
	UFUNCTION(BlueprintCallable)
	void PlayHitGroundSound();

	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
};
