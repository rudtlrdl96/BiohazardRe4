// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Map/BGeometryInteractionActor.h"

bool ABGeometryInteractionActor::AbleInteraction() const
{
	// Todo : 지오메트리 기능 참조해서 부서지지 않은 상태에서만 호출

	return true;
}

EInteraction ABGeometryInteractionActor::GetInteractionType() const
{
	return EInteraction::BreakBox;
}
