// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Monster/BChainsawMan.h"

ABChainsawMan::ABChainsawMan()
{
	GetMesh()->SetRelativeLocation(FVector(0.0f, 0.0f, -88.0f));

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> ChainsawManSkeletalMeshRef(TEXT("/Script/Engine.SkeletalMesh'/Game/Assets/Monster/Mesh/SK_ChainsawMan.SK_ChainsawMan'"));
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> ChainsawSkeletalMeshRef(TEXT("/Script/Engine.SkeletalMesh'/Game/Assets/Monster/Mesh/SK_Chainsaw.SK_Chainsaw'"));
	static ConstructorHelpers::FClassFinder<UAnimInstance> ChainsawManAnimInstaceClassRef(TEXT("/Game/Blueprints/Actor/Monster/ABP_ChainsawMan.ABP_ChainsawMan_C"));
	
	if (ChainsawManSkeletalMeshRef.Object != nullptr)
	{
		GetMesh()->SetSkeletalMesh(ChainsawManSkeletalMeshRef.Object);
	}

	if (ChainsawManAnimInstaceClassRef.Class)
	{
		GetMesh()->SetAnimInstanceClass(ChainsawManAnimInstaceClassRef.Class);
	}

	Weapon = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SK_Chainsaw"));
	Weapon->SetupAttachment(GetMesh(), TEXT("ChainsawSocket"));

	if (ChainsawSkeletalMeshRef.Object != nullptr)
	{
		Weapon->SetSkeletalMesh(ChainsawSkeletalMeshRef.Object);
		Weapon->SetCollisionProfileName(TEXT("NoCollision"));
	}

}

void ABChainsawMan::BeginPlay()
{
	Super::BeginPlay();
}
