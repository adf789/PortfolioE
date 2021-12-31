// Fill out your copyright notice in the Description page of Project Settings.

#include "POEGruxAnimInstance.h"

UPOEGruxAnimInstance::UPOEGruxAnimInstance() {
	IsDead = false;
}

void UPOEGruxAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	auto actor = GetOwningActor();
	if (::IsValid(actor)) {
		PawnSpeed = actor->GetVelocity().SizeSquared();
	}
}

void UPOEGruxAnimInstance::AnimNotify_SpawnEnd()
{
	OnSpawnEnd.Broadcast();
}
