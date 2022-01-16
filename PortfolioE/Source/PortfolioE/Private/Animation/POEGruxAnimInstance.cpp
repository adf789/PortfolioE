// Fill out your copyright notice in the Description page of Project Settings.

#include "POEGruxAnimInstance.h"
#include "POEGrux_Boss.h"
#include <cstdlib>

const int8 UPOEGruxAnimInstance::MaxMeleeAttackIndex = 2;

UPOEGruxAnimInstance::UPOEGruxAnimInstance() {
	static ConstructorHelpers::FObjectFinder<UAnimMontage>
		GRUX_ATTACK1_MONTAGE(TEXT("/Game/POE/Animations/Grux/GruxAttack_Montage.GruxAttack_Montage"));
	if (GRUX_ATTACK1_MONTAGE.Succeeded()) {
		GruxMeleeAttack1Anim = GRUX_ATTACK1_MONTAGE.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage>
		GRUX_ATTACK2_MONTAGE(TEXT("/Game/POE/Animations/Grux/GruxDoubleAttack_Montage.GruxDoubleAttack_Montage"));
	if (GRUX_ATTACK2_MONTAGE.Succeeded()) {
		GruxMeleeAttack2Anim = GRUX_ATTACK2_MONTAGE.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage>
		GRUX_HIT_MONTAGE(TEXT("/Game/POE/Animations/Grux/GruxHit_Montage.GruxHit_Montage"));
	if (GRUX_HIT_MONTAGE.Succeeded()) {
		GruxHitAnim = GRUX_HIT_MONTAGE.Object;
	}
}

void UPOEGruxAnimInstance::PlayAttackMotion()
{
	Super::PlayAttackMotion();

	if (Montage_IsPlaying(GruxHitAnim)) return;
	else if (PlayedAttackAnimIndex == 2 && Montage_IsPlaying(GruxMeleeAttack2Anim)) return;
	else if (Montage_IsPlaying(GruxMeleeAttack1Anim)) return;

	PlayedAttackAnimIndex = FMath::RoundToInt(FMath::RandRange(1.0f, 2.0f));

	if (PlayedAttackAnimIndex == 2) {
		Montage_Play(GruxMeleeAttack2Anim);
	}
	else {
		Montage_Play(GruxMeleeAttack1Anim);
	}
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
}
