// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BCollisionObserver.generated.h"

DECLARE_DELEGATE_OneParam(CollisionEnterDelegate, AActor*)
DECLARE_DELEGATE_OneParam(CollisionExitDelegate, AActor*)

UCLASS(Abstract)
class BIOHAZARDRE4_API ABCollisionObserver : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABCollisionObserver();


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
public:
	CollisionEnterDelegate CollisionEnterCallback;
	CollisionExitDelegate CollisionExitCallback;

	UFUNCTION()
	void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	void SetCollisionProfileName(FName InCollisionProfileName, bool bUpdateOverlaps = true);

	void SetVisibilityCollision(bool _IsActive);

	virtual float TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

protected:
	UPROPERTY(EditAnywhere)
	UShapeComponent* CollisionComp = nullptr;

	FName CollisionProfileName = "";

	virtual void CreateCollision() PURE_VIRTUAL(ABCollisionObserver::CreateCollision, return;);
};
