// Fill out your copyright notice in the Description page of Project Settings.


#include "Generic/BCollisionObserver.h"
#include "Components/ShapeComponent.h"
#include "BiohazardRe4.h"

// Sets default values
ABCollisionObserver::ABCollisionObserver()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
} 

// Called when the game starts
void ABCollisionObserver::BeginPlay()
{
	Super::BeginPlay();
}

void ABCollisionObserver::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (false == CollisionEnterCallback.IsBound())
	{
		return;
	}

	CollisionEnterCallback.Execute(OtherActor);
}

void ABCollisionObserver::OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (false == CollisionExitCallback.IsBound())
	{
		return;
	}

	CollisionExitCallback.Execute(OtherActor);
}

void ABCollisionObserver::SetCollisionProfileName(FName InCollisionProfileName, bool bUpdateOverlaps)
{
	CollisionProfileName = InCollisionProfileName;

	CollisionComp->SetCollisionProfileName(CollisionProfileName, bUpdateOverlaps);
}

void ABCollisionObserver::SetVisibilityCollision(bool _IsActive)
{
	CollisionComp->SetVisibility(_IsActive);

	if (true == _IsActive)
	{
		CollisionComp->SetCollisionProfileName(CollisionProfileName);
	}
	else
	{
		CollisionComp->SetCollisionProfileName("NoCollision");
	}
}

float ABCollisionObserver::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float dmg = Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);

	return 0.0f;
}