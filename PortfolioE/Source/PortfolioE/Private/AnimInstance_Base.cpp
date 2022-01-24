// Fill out your copyright notice in the Description page of Project Settings.

#include "AnimInstance_Base.h"

UAnimInstance_Base::UAnimInstance_Base() {
	PawnSpeed = .0f;
	IsDead = false;
	BufferedCalculateCount = 0;
}

void UAnimInstance_Base::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	auto Pawn = TryGetPawnOwner();
	if (::IsValid(Pawn)) {
		PrevForwardVector = Pawn->GetActorForwardVector();
	}
}

void UAnimInstance_Base::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	auto Pawn = TryGetPawnOwner();
	if (::IsValid(Pawn)) {
		if (!IsDead) {
			PawnSpeed = Pawn->GetVelocity().Size();

			FVector DirectVector = Pawn->GetActorForwardVector() - PrevForwardVector;
			FVector CrossVector = FVector::CrossProduct(Pawn->GetActorForwardVector(), PrevForwardVector);

			// Calculate degree value
			if (DirectVector.Size() == .0f) {
				if (BufferedCalculateCount == MaxBufferedCalculateCount) {
					BufferedCalculateCount = 0;

					if (FMath::Abs(PawnRotateValue) <= 1.0f) PawnRotateValue = .0f;
					else PawnRotateValue += (PawnRotateValue > .0f ? -1.0f : 1.0f);
				}
				else {
					BufferedCalculateCount++;
				}
			}
			else {
				BufferedCalculateCount = 0;
				PawnRotateValue = DirectVector.Size() * 100 * (CrossVector.Z >= 0 ? -1.0f : 1.0f);
			}
			PrevForwardVector = Pawn->GetActorForwardVector();

		}
		else {
			PawnSpeed = .0f;
			PawnRotateValue = .0f;
		}
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

void UAnimInstance_Base::BindCharacter(APOECharacter_Base* Actor)
{
	CharacterActor = Actor;
}

void UAnimInstance_Base::PlayAttackMotion()
{
}

void UAnimInstance_Base::PlayHitMotion(FVector Direction)
{
}

void UAnimInstance_Base::AnimNotify_SpawnEnd()
{
	OnSpawnEnd.Broadcast();
}

void UAnimInstance_Base::AnimNotify_AttackEnd()
{
	OnAttackEnd.Broadcast();
}
