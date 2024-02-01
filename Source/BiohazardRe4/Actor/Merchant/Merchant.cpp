// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Merchant/Merchant.h"

// Sets default values
ABMerchant::ABMerchant()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ABMerchant::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ABMerchant::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

