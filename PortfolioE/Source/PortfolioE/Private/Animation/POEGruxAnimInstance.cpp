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
}

void UPOEGruxAnimInstance::AnimNotify_SpawnEnd()
{
	OnSpawnEnd.Broadcast();
}

void UPOEGruxAnimInstance::AnimNotify_AttackEnd()
{
	OnAttackEnd.Broadcast();
}
