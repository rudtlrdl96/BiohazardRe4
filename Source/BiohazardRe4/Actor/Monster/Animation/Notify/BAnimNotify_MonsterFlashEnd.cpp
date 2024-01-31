// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Monster/Animation/Notify/BAnimNotify_MonsterFlashEnd.h"
#include "GameFramework/Character.h"
#include "BiohazardRe4.h"

void UBAnimNotify_MonsterFlashEnd::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	AActor* MyActor = MeshComp->GetOwner();
	if (MyActor == nullptr)
	{
		LOG_WARNING(TEXT("MyActor is nullptr"));
		return;
	}

	ACharacter* MyCharacter = Cast<ACharacter>(MyActor);
	if (MyCharacter == nullptr)
	{
		LOG_WARNING(TEXT("MyCharacter is nullptr"));
		return;
	}

	USkeletalMeshComponent* MyMesh = MyCharacter->GetMesh();
	if (MyMesh == nullptr)
	{
		LOG_WARNING(TEXT("MyMesh is nullptr"));
		return;
	}

	UAnimInstance* MyAnimInstance = MyMesh->GetAnimInstance();
	if (MyAnimInstance == nullptr)
	{
		LOG_WARNING(TEXT("MyAnimInstance is nullptr"));
		return;
	}

	FName CurSection = MyAnimInstance->Montage_GetCurrentSection();
	FString CurSectionStr = CurSection.ToString();

	LOG_MSG(TEXT("%s"), *CurSectionStr);

	FString CurSectionNumberStr = CurSectionStr.Mid(CurSectionStr.Len() - 1, 1);
	int CurSectionNumberInt = FCString::Atoi(*CurSectionNumberStr);

	FString NextSectionStr = FString::Printf(TEXT("FlashEnd%d"), CurSectionNumberInt);
	FName NextSection(NextSectionStr);
	UAnimMontage* CurMontage = MyAnimInstance->GetCurrentActiveMontage();
	
	if (CurMontage->IsValidSectionName(NextSection) == false)
	{
		LOG_WARNING(TEXT("SectionName is Invalid"));
		return;
	}

	MyAnimInstance->Montage_JumpToSection(NextSection, CurMontage);
}
