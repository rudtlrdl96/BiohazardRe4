// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryItem.h"
#include "Generic/BFsm.h"
#include "Components/WidgetComponent.h"

ABInventoryItem::ABInventoryItem()
{
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Mesh->SetupAttachment(RootComponent);

	// FSM
	FSMComp = CreateDefaultSubobject<UBFsm>(TEXT("FSM"));
	
	UBFsm::FStateCallback WaitState;
	UBFsm::FStateCallback MoveState;
	UBFsm::FStateCallback PutState;
	MoveState.EnterDel.BindUObject(this, &ABInventoryItem::MoveEnter);
	MoveState.UpdateDel.BindUObject(this, &ABInventoryItem::MoveUpdate);
	PutState.EnterDel.BindUObject(this, &ABInventoryItem::PutEnter);
	PutState.UpdateDel.BindUObject(this, &ABInventoryItem::PutUpdate);

	FSMComp->CreateState(TO_KEY(ItemState::Wait), WaitState);
	FSMComp->CreateState(TO_KEY(ItemState::Move), MoveState);
	FSMComp->CreateState(TO_KEY(ItemState::Put), PutState);

	FSMComp->ChangeState(TO_KEY(ItemState::Wait));
}

void ABInventoryItem::SetItemData(const FBItemData& _Data)
{
	Data = _Data;
	Mesh->RegisterComponent();
	Mesh->SetStaticMesh(_Data.Mesh);
	MeshLocation = _Data.Location;
	Mesh->SetRelativeLocation(_Data.Location);
	Mesh->SetRelativeRotation(_Data.Rotation);
	Mesh->SetRelativeScale3D(_Data.Scale);
	SetItemNumText();
}

void ABInventoryItem::SetRaise()
{
	StartLocation = RootComponent->GetRelativeLocation();
	TargetLocation = StartLocation;
	FSMComp->ChangeState(TO_KEY(ItemState::Move));
}

void ABInventoryItem::SetMove(const FVector& _Location)
{
	StartLocation = RootComponent->GetRelativeLocation();
	TargetLocation = _Location;
	MoveAlpha = 0;
}

void ABInventoryItem::SetPut(const FVector& _Location)
{
	StartLocation = RootComponent->GetRelativeLocation();
	TargetLocation = _Location;
	FSMComp->ChangeState(TO_KEY(ItemState::Put));
}

void ABInventoryItem::Turn()
{
	bIsTurn = ~bIsTurn;
	bIsCurrentTurn = 1;
	TurnAlpha = 0;
	if (bIsTurn)
	{
		MeshStartLocation = Data.Location;
		MeshStartRotate = Data.Rotation;
		MeshTargetLocation = Data.TurnLocation;
		MeshTargetRotate = Data.TurnRotation;
	}
	else
	{
		MeshStartLocation = Data.TurnLocation;
		MeshStartRotate = Data.TurnRotation;
		MeshTargetLocation = Data.Location;
		MeshTargetRotate = Data.Rotation;
	}

	SetItemNumText();
}

void ABInventoryItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsCurrentTurn)
	{
		TurnAlpha += DeltaTime * TurnSpeed;
		MeshLocation = FMath::Lerp(MeshStartLocation, MeshTargetLocation, TurnAlpha);
		Mesh->SetRelativeRotation(FMath::LerpRange(MeshStartRotate, MeshTargetRotate, TurnAlpha));
		if (1 < TurnAlpha)
		{
			Mesh->SetRelativeRotation(MeshTargetRotate);
			bIsCurrentTurn = 0;
		}
	}
}

void ABInventoryItem::MoveEnter()
{
	MoveAlpha = 0;
	RaiseAlpha = 0;
	StartLocation = RootComponent->GetRelativeLocation();
}

void ABInventoryItem::MoveUpdate(float DeltaTime)
{
	MoveAlpha = FMath::Min(1.0f, MoveAlpha + DeltaTime * MoveSpeed);
	RaiseAlpha = FMath::Min(1.0f, RaiseAlpha + DeltaTime * RaiseSpeed);
	
	RootComponent->SetRelativeLocation(FMath::Lerp<FVector>(StartLocation, TargetLocation, MoveAlpha));
	Mesh->SetRelativeLocation(FMath::Lerp<FVector>(MeshLocation, MeshLocation + FVector(0, 0, 5), RaiseAlpha));
}

void ABInventoryItem::PutEnter()
{
	MoveAlpha = 0;
	RaiseAlpha = 0;
	StartLocation = RootComponent->GetRelativeLocation();
}

void ABInventoryItem::PutUpdate(float DeltaTime)
{
	MoveAlpha = FMath::Min(1.0f, MoveAlpha + DeltaTime * MoveSpeed);
	RaiseAlpha = FMath::Min(1.0f, RaiseAlpha + DeltaTime * RaiseSpeed);

	RootComponent->SetRelativeLocation(FMath::Lerp<FVector>(StartLocation, TargetLocation, MoveAlpha));
	Mesh->SetRelativeLocation(FMath::Lerp<FVector>(MeshLocation + FVector(0, 0, 5), MeshLocation, RaiseAlpha));

	if (.99f <= RaiseAlpha)
	{
		RootComponent->SetRelativeLocation(TargetLocation);
		Mesh->SetRelativeLocation(MeshLocation);
		FSMComp->ChangeState(TO_KEY(ItemState::Wait));
	}
}
