// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Map/BGeometryInteractionActor.h"

bool ABGeometryInteractionActor::AbleInteraction() const
{
	// Todo : ������Ʈ�� ��� �����ؼ� �μ����� ���� ���¿����� ȣ��

	return true;
}

EInteraction ABGeometryInteractionActor::GetInteractionType() const
{
	return EInteraction::BreakBox;
}
