// Fill out your copyright notice in the Description page of Project Settings.

#include "POEMonster_Base.h"
#include "Animinstance_Base.h"
#include "POEMonsterAIController.h"
#include "BehaviorTree/BlackboardComponent.h"

APOEMonster_Base::APOEMonster_Base() {
	AIControllerClass = APOEMonsterAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	IsSpawned = false;
}

void APOEMonster_Base::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	GetCharacterMovement()->MaxWalkSpeed = MaxSpeed;

	APOEMonsterAIController* AIController = Cast<APOEMonsterAIController>(GetController());
	CHECKRETURN(AIController == nullptr);
	AnimInstance->OnSpawnEnd.AddUObject(AIController, &APOEMonsterAIController::RunAI);

	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bUseControllerDesiredRotation = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(.0f, 480.0f, .0f);
}

float APOEMonster_Base::TakeDamage(float DamageAmount, FDamageEvent const & DamageEvent, AController * EventInstigator, AActor * DamageCauser)
{
	float TempDamage = DamageAmount - CharacterStatus->DefenseValue;
	if (TempDamage < KINDA_SMALL_NUMBER) TempDamage = .0f;

	Super::TakeDamage(TempDamage, DamageEvent, EventInstigator, DamageCauser);
	return TempDamage;
}

float APOEMonster_Base::GetAIDetectRange()
{
	return AIDetectDistance;
}
float APOEMonster_Base::GetAttackDelay()
{
	return DelayAfterAttack;
}
\
