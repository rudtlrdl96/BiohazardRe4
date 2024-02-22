// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Player/Weapon/Knife/BLeonKnife.h"
#include "Generic/BCollisionObserverCapsule.h"
#include "Kismet/GameplayStatics.h"

void ABLeonKnife::BeginPlay()
{
	Super::BeginPlay();

	FActorSpawnParameters Params;
	Params.Owner = this;

	AttackCollision = GetWorld()->SpawnActor<ABCollisionObserverCapsule>(Params);

	AttackCollision->AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform);
	AttackCollision->SetActorRelativeLocation(FVector(0, 0, 0));
	AttackCollision->SetActorRelativeRotation(FRotator(0, 0, 0));

	AttackCollision->CollisionEnterCallback.BindUObject(this, &ABLeonKnife::KnifeAttack);

	AttackCollision->SetCollisionProfileName("Interaction");
	ActiveCollision(false);
}

void ABLeonKnife::ActiveCollision(bool _IsActive)
{
	bCollisionActive = _IsActive;
	bIsKnifeAttackSoundPlay = true;
	AttackCollision->SetVisibilityCollision(_IsActive);
}

void ABLeonKnife::KnifeAttack(AActor* _OverlapActor)
{
	if (false == bCollisionActive)
	{
		return;
	}

	if (_OverlapActor == GetOwner())
	{
		return;
	}

	float Damage = UGameplayStatics::ApplyDamage(_OverlapActor, 100, nullptr, this, DamageType);

	LOG_MSG(TEXT("Damage : %f"), Damage);

	if (0.0f < Damage && true == bIsKnifeAttackSoundPlay)
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), KnifeHitSound, GetActorLocation());
		bIsKnifeAttackSoundPlay = false;
	}
}
