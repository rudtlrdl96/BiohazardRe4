// Fill out your copyright notice in the Description page of Project Settings.


#include "Generic/BCollisionObserver.h"
#include "Components/SphereComponent.h"

// Sets default values for this component's properties
UBCollisionObserver::UBCollisionObserver()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	CollosionComp = CreateDefaultSubobject<USphereComponent>(TEXT("Overlap Colliision"));
	CollosionComp->SetCollisionProfileName("OverlapAll");
}

void UBCollisionObserver::SetLocation(const FVector& _Location)
{
	CollosionComp->SetRelativeLocation(_Location);
}

void UBCollisionObserver::SetScale(float _Radius)
{
	CollosionComp->SetSphereRadius(_Radius);
}

// Called when the game starts
void UBCollisionObserver::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

// Called every frame
void UBCollisionObserver::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UBCollisionObserver::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

}

void UBCollisionObserver::OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{

}