// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BCollisionObserver.generated.h"

DECLARE_DELEGATE_OneParam(CollisionEnterDelegate, AActor*)
DECLARE_DELEGATE_OneParam(CollisionExitDelegate, AActor*)

class USphereComponent;

UCLASS()
class BIOHAZARDRE4_API ABCollisionObserver : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABCollisionObserver();

	void SetCollisionProfileName(FName InCollisionProfileName, bool bUpdateOverlaps = true);

	UFUNCTION()
	void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	UFUNCTION()
	void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UPROPERTY(EditAnywhere, Category = "Collision")
	USphereComponent* CollosionComp = nullptr;

	CollisionEnterDelegate CollisionEnterCallback;
	CollisionExitDelegate CollisionExitCallback;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
};
