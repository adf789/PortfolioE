// Fill out your copyright notice in the Description page of Project Settings.

#include "POEGrux_Boss.h"
#include "POEMonsterAIController.h"
#include "POEGruxAnimInstance.h"

APOEGrux_Boss::APOEGrux_Boss() {
	static ConstructorHelpers::FObjectFinder<USkeletalMesh>
		SM_GRUX(TEXT("/Game/ParagonGrux/Characters/Heroes/Grux/Meshes/Grux.Grux"));
	if (SM_GRUX.Succeeded()) {
		GetMesh()->SetSkeletalMesh(SM_GRUX.Object);
		GetMesh()->SetWorldLocationAndRotation(FVector(.0f, .0f, -90.0f), FRotator(.0f, -90.0f, .0f));
	}

	static ConstructorHelpers::FClassFinder<UAnimInstance>
		ANIM_GRUX_C(TEXT("/Game/POE/Animations/Grux/Grux.Grux_C"));
	if (ANIM_GRUX_C.Succeeded()) {
		GetMesh()->SetAnimInstanceClass(ANIM_GRUX_C.Class);
	}

	AIControllerClass = APOEMonsterAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	IsSpawned = false;
}

void APOEGrux_Boss::BeginPlay()
{
	Super::BeginPlay();
}

void APOEGrux_Boss::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APOEGrux_Boss::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	GetCharacterMovement()->MaxWalkSpeed = 300.0f;

	auto GruxAnim = Cast<UPOEGruxAnimInstance>(AnimInstance);
	CHECKRETURN(GruxAnim == nullptr);

	AIController = Cast<APOEMonsterAIController>(GetController());
	CHECKRETURN(AIController == nullptr);
	GruxAnim->OnSpawnEnd.AddUObject(AIController, &APOEMonsterAIController::RunAI);

	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bUseControllerDesiredRotation = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(.0f, 480.0f, .0f);
}

float APOEGrux_Boss::GetAttackRange()
{
	return 230.0f;
}
