// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryItem.h"
#include "Generic/BFsm.h"

// Sets default values for this component's properties
UBInventoryItem::UBInventoryItem()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

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
	MeshLocation = _Data.Location;
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

void UBInventoryItem::Turn()
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
}

void UBInventoryItem::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

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
	Mesh->SetRelativeLocation(FMath::Lerp<FVector>(MeshLocation, MeshLocation + FVector(0, 0, 5), RaiseAlpha));
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
	Mesh->SetRelativeLocation(FMath::Lerp<FVector>(MeshLocation + FVector(0, 0, 5), MeshLocation, RaiseAlpha));

	if (.99f <= RaiseAlpha)
	{
		SetRelativeLocation(TargetLocation);
		Mesh->SetRelativeLocation(MeshLocation);
		FSMComp->ChangeState(TO_KEY(ItemState::Wait));
	}
}
