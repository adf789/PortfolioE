// Fill out your copyright notice in the Description page of Project Settings.

#include "POELotteryMonster_Anim.h"

UPOELotteryMonster_Anim::UPOELotteryMonster_Anim() {
	static ConstructorHelpers::FObjectFinder<UAnimMontage>
		HIT_MONTAGE(TEXT("/Game/POE/Animations/Monster/MonsterLotteryHit.MonsterLotteryHit"));
	if (HIT_MONTAGE.Succeeded()) {
		HitAnim = HIT_MONTAGE.Object;
	}
}

void UPOELotteryMonster_Anim::PlayHitMotion(FVector Direction)
{
	Super::PlayHitMotion(Direction);

	if (Montage_IsPlaying(HitAnim)) return;

	Montage_Play(HitAnim);
}
