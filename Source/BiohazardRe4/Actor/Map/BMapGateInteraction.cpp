// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Map/BMapGateInteraction.h"
#include "Components/SceneComponent.h"
#include "Components/BoxComponent.h"

ABMapGateInteraction::ABMapGateInteraction()
{
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	ATrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("Trigger A"));
	ATrigger->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	ATrigger->SetRelativeLocation(FVector(FVector(-65, 0, 0)));
	ATrigger->SetBoxExtent(FVector(10, 100, 10));
	ATrigger->SetCollisionProfileName("Interaction");

}
