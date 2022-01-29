// Fill out your copyright notice in the Description page of Project Settings.

#include "POEGrux_Boss.h"

APOEGrux_Boss::APOEGrux_Boss() {
	static ConstructorHelpers::FObjectFinder<USkeletalMesh>
		SM_GRUX(TEXT("/Game/ParagonGrux/Characters/Heroes/Grux/Meshes/Grux.Grux"));
	if (SM_GRUX.Succeeded()) {
		GetMesh()->SetSkeletalMesh(SM_GRUX.Object);
		GetMesh()->SetWorldLocationAndRotation(FVector(.0f, .0f, -90.0f), FRotator(.0f, -90.0f, .0f));
		GetMesh()->SetCollisionProfileName(TEXT("NoCollision"));
	}

	static ConstructorHelpers::FClassFinder<UAnimInstance>
		ANIM_GRUX_C(TEXT("/Game/POE/Blueprints/Animations/Grux.Grux_C"));
	if (ANIM_GRUX_C.Succeeded()) {
		GetMesh()->SetAnimInstanceClass(ANIM_GRUX_C.Class);
	}

	InitializeValues();
}

void APOEGrux_Boss::InitializeValues()
{
	GetCapsuleComponent()->SetWorldScale3D(FVector(2.0f, 2.0f, 2.0f));

	CharacterStatus->InitValue(10000.0f, 5.0f, 100.0f);
	MaxSpeed = 400.0f;
	AttackRange = 400.0f;
	AIDetectDistance = 2000.0f;
	AttackCollisionScale = 50.0f;
}
