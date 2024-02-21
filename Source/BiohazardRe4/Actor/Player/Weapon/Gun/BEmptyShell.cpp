// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Player/Weapon/Gun/BEmptyShell.h"
#include "BiohazardRe4.h"
#include "Kismet/GameplayStatics.h"

ABEmptyShell::ABEmptyShell()
{
	GetStaticMeshComponent()->Mobility = EComponentMobility::Movable;
	GetStaticMeshComponent()->SetSimulatePhysics(true);
	GetStaticMeshComponent()->OnComponentHit.AddDynamic(this, &ABEmptyShell::OnHit);
	GetStaticMeshComponent()->SetNotifyRigidBodyCollision(true);
	PrimaryActorTick.bCanEverTick = true;
}

void ABEmptyShell::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	Time += DeltaTime;
	if (Time >= LifeTime)
	{
		Destroy();
	}
}

void ABEmptyShell::PlayHitGroundSound()
{
	if (Sound_HitGround == nullptr)
	{
		LOG_WARNING(TEXT("Sound_HitGround == nullptr"))
			return;
	}

	UGameplayStatics::PlaySoundAtLocation(GetWorld(), Sound_HitGround, GetActorLocation(), GetActorRotation());
}

void ABEmptyShell::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	LOG_WARNING(TEXT("Call OnHit"))
	PlayHitGroundSound();
	GetStaticMeshComponent()->SetNotifyRigidBodyCollision(false);
}

