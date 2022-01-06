// Fill out your copyright notice in the Description page of Project Settings.

#include "POEGruxAnimInstance.h"
#include <cstdlib>

const FString UPOEGruxAnimInstance::MeleeAttackNames[] = {TEXT("Default"), TEXT("Attack_Right"), TEXT("Attack_Double")};
const int8 UPOEGruxAnimInstance::MaxMeleeAttackIndex = 2;

UPOEGruxAnimInstance::UPOEGruxAnimInstance() {
	IsDead = false;
	CurMeleeAttackIndex = 1;

	static ConstructorHelpers::FObjectFinder<UAnimMontage>
		GRUX_ATTACK1_MONTAGE(TEXT("/Game/POE/Animations/Grux/GruxAttack_Montage.GruxAttack_Montage"));
	if (GRUX_ATTACK1_MONTAGE.Succeeded()) {
		GruxMeleeAttackAnim = GRUX_ATTACK1_MONTAGE.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage>
		GRUX_HIT_MONTAGE(TEXT("/Game/POE/Animations/Grux/GruxHit_Montage.GruxHit_Montage"));
	if (GRUX_HIT_MONTAGE.Succeeded()) {
		GruxHitAnim = GRUX_HIT_MONTAGE.Object;
	}
}

FName UPOEGruxAnimInstance::GetMeleeAttackName(int8 Section)
{
	if (FMath::IsWithinInclusive<int8>(Section, 1, 3)) {
		return FName(*MeleeAttackNames[Section - 1]);
	}
	return NAME_None;
}

void UPOEGruxAnimInstance::PlayAttackMotion()
{
	Super::PlayAttackMotion();

	Montage_Play(GruxMeleeAttackAnim);
	/*Montage_JumpToSection(GetMeleeAttackName(CurMeleeAttackIndex++), GruxMeleeAttackAnim);
	if (CurMeleeAttackIndex > MaxMeleeAttackIndex) CurMeleeAttackIndex = 1;*/
}

void UPOEGruxAnimInstance::PlayHitMotion(FVector Direction)
{
	Super::PlayHitMotion(Direction);
	if (Montage_IsPlaying(GruxHitAnim)) return;
	FVector DirNormalized = Direction.GetSafeNormal();
	float DotProduct = FVector::DotProduct(DirNormalized, GetOwningActor()->GetActorForwardVector().GetSafeNormal());

	Montage_Play(GruxHitAnim);
	if (DotProduct <= 1 && DotProduct > 0.5) {
		Montage_JumpToSection(TEXT("Hit_Back"), GruxHitAnim);
	}
	else if (DotProduct >= -1 && DotProduct < -0.5) {
		// Basic
	}
}

void UPOEGruxAnimInstance::AnimNotify_SpawnEnd()
{
	OnSpawnEnd.Broadcast();
}

void UPOEGruxAnimInstance::AnimNotify_AttackEnd()
{
	OnAttackEnd.Broadcast();
}
