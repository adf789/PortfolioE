// Fill out your copyright notice in the Description page of Project Settings.

#include "AnimNotifyState_CharacterAttack.h"
#include "POECharacter_Base.h"

void UAnimNotifyState_CharacterAttack::NotifyTick(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float FrameDeltaTime) {
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime);

	APOECharacter_Base* Actor = Cast<APOECharacter_Base>(MeshComp->GetOwner());
	if (Actor != nullptr && ::IsValid(Actor)) {
		Actor->CheckMeleeAttackCollision();
	}
}


