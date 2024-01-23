// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Monster/BMonsterMale1.h"
#include "AIController/BAIBasicMonsterController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Component/BMonsterStatComponent.h"

ABMonsterMale1::ABMonsterMale1()
{
	AIControllerClass = ABAIBasicMonsterController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorld;

	GetCharacterMovement()->MaxWalkSpeed = 50.0f;
	
	Stat->SetAttackRadius(80.0f);
}

