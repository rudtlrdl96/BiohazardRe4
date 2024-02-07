// Fill out your copyright notice in the Description page of Project Settings.


#include "Generic/BCollisionObserverCapsule.h"
#include "Components/CapsuleComponent.h"

ABCollisionObserverCapsule::ABCollisionObserverCapsule()
{
	CreateCollision();
	CollisionComp->OnComponentBeginOverlap.AddDynamic(this, &ABCollisionObserver::OnOverlapBegin);
	CollisionComp->OnComponentEndOverlap.AddDynamic(this, &ABCollisionObserver::OnOverlapEnd);
	SetCollisionProfileName("OverlapAll");
	RootComponent = CollisionComp;
}

void ABCollisionObserverCapsule::SetRadius(float _Radius)
{
	CapsuleCol->SetCapsuleRadius(_Radius);
}

void ABCollisionObserverCapsule::SetHeight(float _Height)
{
	CapsuleCol->SetCapsuleHalfHeight(_Height);
}

void ABCollisionObserverCapsule::CreateCollision()
{
	CapsuleCol = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Overlap Colliision"));
	CollisionComp = CapsuleCol;
}