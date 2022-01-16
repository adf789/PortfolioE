// Fill out your copyright notice in the Description page of Project Settings.

#include "POENormalMonster_Anim.h"

UPOENormalMonster_Anim::UPOENormalMonster_Anim() {
	static ConstructorHelpers::FObjectFinder<UAnimMontage>
		ATTACK_MONTAGE(TEXT("/Game/POE/Animations/Monster/MonsterNormal1Attack_Montage.MonsterNormal1Attack_Montage"));
	if (ATTACK_MONTAGE.Succeeded()) {
		MeleeAttackAnim = ATTACK_MONTAGE.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage>
		HIT_MONTAGE(TEXT("/Game/POE/Animations/Monster/MonsterNormal1Hit_Montage.MonsterNormal1Hit_Montage"));
	if (HIT_MONTAGE.Succeeded()) {
		HitAnim = HIT_MONTAGE.Object;
	}
}

void UPOENormalMonster_Anim::PlayAttackMotion()
{
	Super::PlayAttackMotion();

	if (Montage_IsPlaying(HitAnim) || Montage_IsPlaying(MeleeAttackAnim)) return;
	Montage_Play(MeleeAttackAnim);
}

void UPOENormalMonster_Anim::PlayHitMotion(FVector Direction)
{
	Super::PlayHitMotion(Direction);

	if (Montage_IsPlaying(HitAnim)) return;

	Montage_Play(HitAnim);
}
