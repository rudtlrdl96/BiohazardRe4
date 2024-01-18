// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryItem.h"
#include "Generic/BFsm.h"

// Sets default values for this component's properties
UBInventoryItem::UBInventoryItem()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(this);
	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	FSMComp = CreateDefaultSubobject<UBFsm>(TEXT("FSM"));

	UBFsm::FStateCallback WaitState;
	UBFsm::FStateCallback MoveState;
	UBFsm::FStateCallback PutState;
	MoveState.EnterDel.BindUObject(this, &UBInventoryItem::MoveEnter);
	MoveState.UpdateDel.BindUObject(this, &UBInventoryItem::MoveUpdate);
	PutState.EnterDel.BindUObject(this, &UBInventoryItem::PutEnter);
	PutState.UpdateDel.BindUObject(this, &UBInventoryItem::PutUpdate);

	FSMComp->CreateState(TO_KEY(ItemState::Wait), WaitState);
	FSMComp->CreateState(TO_KEY(ItemState::Move), MoveState);
	FSMComp->CreateState(TO_KEY(ItemState::Put), PutState);

	FSMComp->ChangeState(TO_KEY(ItemState::Wait));
}

void UBInventoryItem::SetItemData(const FBItemData& _Data)
{
	Data = _Data;
	Mesh->RegisterComponent();
	Mesh->SetStaticMesh(_Data.Mesh);
	Mesh->SetRelativeLocation(_Data.Location);
	Mesh->SetRelativeRotation(_Data.Rotation);
	Mesh->SetRelativeScale3D(_Data.Scale);
}

void UBInventoryItem::SetRaise()
{
	StartLocation = GetRelativeLocation();
	TargetLocation = StartLocation;
	FSMComp->ChangeState(TO_KEY(ItemState::Move));
}

void UBInventoryItem::SetMove(const FVector& _Location)
{
	StartLocation = GetRelativeLocation();
	TargetLocation = _Location;
	MoveAlpha = 0;
}

void UBInventoryItem::SetPut(const FVector& _Location)
{
	StartLocation = GetRelativeLocation();
	TargetLocation = _Location;
	FSMComp->ChangeState(TO_KEY(ItemState::Put));
}

void UBInventoryItem::MoveEnter()
{
	MoveAlpha = 0;
	RaiseAlpha = 0;
	StartLocation = GetRelativeLocation();
}
void UBInventoryItem::MoveUpdate(float DeltaTime)
{
	MoveAlpha = FMath::Min(1.0f, MoveAlpha + DeltaTime * MoveSpeed);
	RaiseAlpha = FMath::Min(1.0f, RaiseAlpha + DeltaTime * RaiseSpeed);
	
	SetRelativeLocation(FMath::Lerp<FVector>(StartLocation, TargetLocation, MoveAlpha));
	Mesh->SetRelativeLocation(FMath::Lerp<FVector>(Data.Location, Data.Location + FVector(0, 0, 5), RaiseAlpha));
}

void UBInventoryItem::PutEnter()
{
	MoveAlpha = 0;
	RaiseAlpha = 0;
	StartLocation = GetRelativeLocation();
}
void UBInventoryItem::PutUpdate(float DeltaTime)
{
	MoveAlpha = FMath::Min(1.0f, MoveAlpha + DeltaTime * MoveSpeed);
	RaiseAlpha = FMath::Min(1.0f, RaiseAlpha + DeltaTime * RaiseSpeed);

	SetRelativeLocation(FMath::Lerp<FVector>(StartLocation, TargetLocation, MoveAlpha));
	Mesh->SetRelativeLocation(FMath::Lerp<FVector>(Data.Location + FVector(0, 0, 5), Data.Location, RaiseAlpha));

	if (.99f <= RaiseAlpha)
	{
		SetRelativeLocation(TargetLocation);
		Mesh->SetRelativeLocation(Data.Location);
		FSMComp->ChangeState(TO_KEY(ItemState::Wait));
	}
}
