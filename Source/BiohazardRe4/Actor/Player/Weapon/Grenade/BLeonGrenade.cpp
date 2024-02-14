// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Player/Weapon/Grenade/BLeonGrenade.h"
#include "Generic/BCollisionObserverSphere.h"
#include "DamageType/PlayerDamageType/BDMGPlayerRifle.h"
#include "Kismet/GameplayStatics.h"


ABLeonGrenade::ABLeonGrenade()
	: IgnoreDamage(TArray<AActor*, FDefaultAllocator>())
{
	DamageType = UBDMGPlayerRifle::StaticClass();
}

void ABLeonGrenade::PostActorCreated()
{

}

void ABLeonGrenade::BeginPlay()
{
	Super::BeginPlay();
}

void ABLeonGrenade::Tick(float DeltaTime)
{
	if (IsThrowing)
	{
		Time += DeltaTime;

		if (Time >= LifeTime)
		{
			Explosion();
		}
	}
}

void ABLeonGrenade::Explosion()
{
	FActorSpawnParameters Params;
	Params.Owner = this;

	ExplosionCollision = GetWorld()->SpawnActor<ABCollisionObserverSphere>(Params);

	ExplosionCollision->AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform);
	ExplosionCollision->SetActorRelativeLocation(FVector(0, 0, 0));
	ExplosionCollision->SetActorRelativeRotation(FRotator(0, 0, 0));
	ExplosionCollision->SetRadius(Radius);
		//SetActorRelativeScale3D(FVector::One() * Radius);

	ExplosionCollision->CollisionEnterCallback.BindUObject(this, &ABLeonGrenade::ExplosionAttack);

	ExplosionCollision->SetCollisionProfileName("Interaction");
	ExplosionCollision->SetVisibilityCollision(true);
	DrawDebugSphere(GetWorld(), ExplosionCollision->GetActorLocation(), Radius, 30, FColor::Green);
	LOG_MSG(TEXT("Explosion"))
	ExplosionCollision->Destroy();
	Destroy();
}

void ABLeonGrenade::ExplosionAttack(AActor* _OverlapActor)
{
	//UGameplayStatics::ApplyDamage(_OverlapActor, 100, Player->GetController(), this, DamageType);
	UGameplayStatics::ApplyRadialDamage
	(
		_OverlapActor,
		100,
		ExplosionCollision->GetActorLocation(),
		Radius,
		DamageType,
		IgnoreDamage,
		this,
		Player->GetController(),
		false,
		ECollisionChannel::ECC_EngineTraceChannel4
	);
}

void ABLeonGrenade::Attack()
{
	IsThrowing = true;
}
