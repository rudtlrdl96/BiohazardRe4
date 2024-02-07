// Fill out your copyright notice in the Description page of Project Settings.


#include "Generic/BCollisionObserver.h"
#include "Components/SphereComponent.h"
#include "BiohazardRe4.h"

// Sets default values
ABCollisionObserver::ABCollisionObserver()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	CollosionComp = CreateDefaultSubobject<USphereComponent>(TEXT("Overlap Colliision"));
	SetCollisionProfileName("OverlapAll");

	CollosionComp->OnComponentBeginOverlap.AddDynamic(this, &ABCollisionObserver::OnOverlapBegin);
	CollosionComp->OnComponentEndOverlap.AddDynamic(this, &ABCollisionObserver::OnOverlapEnd);
	RootComponent = CollosionComp;
}
 
void ABCollisionObserver::SetCollisionProfileName(FName InCollisionProfileName, bool bUpdateOverlaps)
{
	CollosionComp->SetCollisionProfileName(InCollisionProfileName, bUpdateOverlaps);
}

// Called when the game starts
void ABCollisionObserver::BeginPlay()
{
	Super::BeginPlay();

	// ...

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