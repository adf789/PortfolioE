// Fill out your copyright notice in the Description page of Project Settings.

#include "AnimInstance_Base.h"

UAnimInstance_Base::UAnimInstance_Base() {
	PawnSpeed = .0f;
	IsDead = false;
}

void UAnimInstance_Base::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	auto Pawn = TryGetPawnOwner();
	if (::IsValid(Pawn)) {
		if (!IsDead) PawnSpeed = Pawn->GetVelocity().Size();
		else PawnSpeed = .0f;
	}
}

void UAnimInstance_Base::PlayAttack()
{
	if (IsDead) return;
	PlayAttackMotion();
}

void UAnimInstance_Base::PlayHit(FVector Direction)
{
	if (IsDead) return;
	PlayHitMotion(Direction);
}

void UAnimInstance_Base::PlayAttackMotion()
{
}

void UAnimInstance_Base::PlayHitMotion(FVector Direction)
{
}
