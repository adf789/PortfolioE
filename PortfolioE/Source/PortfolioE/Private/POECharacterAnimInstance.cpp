// Fill out your copyright notice in the Description page of Project Settings.

#include "POECharacterAnimInstance.h"

UPOECharacterAnimInstance::UPOECharacterAnimInstance() {
	pawnSpeed = .0f;
	isDead = false;
}

void UPOECharacterAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	auto Pawn = TryGetPawnOwner();
	if (::IsValid(Pawn)) {
		if (!isDead) {
			pawnSpeed = Pawn->GetVelocity().Size();
		}
	}
}
