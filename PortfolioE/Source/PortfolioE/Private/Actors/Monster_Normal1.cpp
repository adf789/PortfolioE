// Fill out your copyright notice in the Description page of Project Settings.

#include "Monster_Normal1.h"

AMonster_Normal1::AMonster_Normal1() {
	static ConstructorHelpers::FObjectFinder<USkeletalMesh>
		SM_GRUX(TEXT("/Game/InfinityBladeWarriors/Character/CompleteCharacters/SK_CharM_Pit.SK_CharM_Pit"));
	if (SM_GRUX.Succeeded()) {
		GetMesh()->SetSkeletalMesh(SM_GRUX.Object);
		GetMesh()->SetWorldLocationAndRotation(FVector(.0f, .0f, -90.0f), FRotator(.0f, -90.0f, .0f));
	}

	static ConstructorHelpers::FClassFinder<UAnimInstance>
		ANIM_GRUX_C(TEXT("/Game/POE/Blueprints/Animations/Grux.Grux_C"));
	if (ANIM_GRUX_C.Succeeded()) {
		GetMesh()->SetAnimInstanceClass(ANIM_GRUX_C.Class);
	}

	CharacterStatus->InitValue(5000.0f, 10.0f, 10.0f);
	AttackDistance = 300.0f;
	AIDetectDistance = 1000.0f;
}


