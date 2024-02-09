// Fill out your copyright notice in the Description page of Project Settings.


#include "BMonsterBase.h"

bool ABMonsterBase::AbleInteraction() const
{
	// Todo : 테스트용 코드 추후 기능완성 후 추가해주세요
	switch (CurState)
	{
		case EMonsterState::Attack:
		case EMonsterState::Flashed:
		case EMonsterState::Damaged:
		{
			return true;
		}
		case EMonsterState::Kicked:
		{
			return false;
		}
		case EMonsterState::Groggy:
		{
			return true;
		}
	}

	return false;
}

EInteraction ABMonsterBase::GetInteractionType() const
{
	switch (CurState)
	{
	case EMonsterState::Idle:
	{
		return EInteraction::None;
	}
	case EMonsterState::Turn:
	{
		return EInteraction::None;
	}
	case EMonsterState::Patrol:
	{
		return EInteraction::None;
	}
	case EMonsterState::Walk:
	{
		return EInteraction::None;
	}
	case EMonsterState::Run:
	{
		return EInteraction::None;
	}
	case EMonsterState::Attack:
	{
		return EInteraction::AttackMonster;
	}
	case EMonsterState::Flashed:
	{
		return EInteraction::GroggyMonster;
	}
	case EMonsterState::Damaged:
	{
		return EInteraction::GroggyMonster;
	}
	case EMonsterState::Death:
	{
		return EInteraction::None;
	}
	case EMonsterState::Kicked:
	{
		return EInteraction::None;
	}
	case EMonsterState::Groggy:
	{
		return EInteraction::GroggyMonster;
	}
	default:
		return EInteraction::None;
	}
}

FVector ABMonsterBase::GetUIPivot() const
{
	return FVector();
}