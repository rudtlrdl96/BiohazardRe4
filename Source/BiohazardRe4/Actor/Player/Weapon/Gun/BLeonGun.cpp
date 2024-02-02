// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Player/Weapon/Gun/BLeonGun.h"
#include "Kismet/GameplayStatics.h"

void ABLeonGun::BeginPlay()
{
	Super::BeginPlay();
	PlayerCamManager = UGameplayStatics::GetPlayerCameraManager(this, 0);
}

bool ABLeonGun::AbleAttack() const
{
	UE_LOG(LogTemp, Log, TEXT("AbleAttack"))
	return true;
}

void ABLeonGun::Attack()
{
	UE_LOG(LogTemp, Log, TEXT("Attack"))
	LineTraceStart = GetActorLocation();
	LineTraceEnd = LineTraceStart + PlayerCamManager->GetActorForwardVector() * Range;

	FCollisionQueryParams LineTraceParams;
	LineTraceParams.AddIgnoredActor(this);
	LineTraceParams.AddIgnoredActor(Player);

	bool bIsHit = GetWorld()->LineTraceSingleByChannel(HitInfo, LineTraceStart, LineTraceEnd, ECollisionChannel::ECC_GameTraceChannel4, LineTraceParams);
	if (bIsHit)
	{
		UE_LOG(LogTemp, Log, TEXT("AttackSuccess"))
		DrawDebugLine(GetWorld(), LineTraceStart, LineTraceEnd, FColor::Green, true, -1.f);
		AActor* ShootedActor = HitInfo.GetActor();
		int a = 0;
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("AttackFail"))
		DrawDebugLine(GetWorld(), LineTraceStart, LineTraceEnd, FColor::Red, true, -1.f);
	}

}

bool ABLeonGun::AbleReload() const
{
	UE_LOG(LogTemp, Log, TEXT("AbleReload"))
	return true;
}

void ABLeonGun::Reload()
{
	UE_LOG(LogTemp, Log, TEXT("Reload"))
}



void ABLeonGun::Shoot()
{
	UE_LOG(LogTemp, Log, TEXT("Shoot"))

}

void ABLeonGun::DropShell()
{
	UE_LOG(LogTemp, Log, TEXT("DropShell"))

}

void ABLeonGun::DropMagazine()
{
	UE_LOG(LogTemp, Log, TEXT("DropMagazine"))

}