// Fill out your copyright notice in the Description page of Project Settings.


#include "Generic/BCollisionObserverSphere.h"
#include "Components/SphereComponent.h"

ABCollisionObserverSphere::ABCollisionObserverSphere()
{
	CreateCollision();
	CollisionComp->OnComponentBeginOverlap.AddDynamic(this, &ABCollisionObserver::OnOverlapBegin);
	CollisionComp->OnComponentEndOverlap.AddDynamic(this, &ABCollisionObserver::OnOverlapEnd);
	SetCollisionProfileName("OverlapAll");
	RootComponent = CollisionComp;
}

void ABCollisionObserverSphere::SetRadius(float _Radius)
{
	SphereCol->SetSphereRadius(_Radius);
}

void ABCollisionObserverSphere::CreateCollision()
{
	SphereCol = CreateDefaultSubobject<USphereComponent>(TEXT("Overlap Colliision"));
	CollisionComp = SphereCol;
}