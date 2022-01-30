// Fill out your copyright notice in the Description page of Project Settings.

#include "POEMonster_Base.h"
#include "Animinstance_Base.h"
#include "POEMonsterAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "DrawDebugHelpers.h"

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

void APOEMonster_Base::CheckMeleeAttackCollision()
{
	Super::CheckMeleeAttackCollision();

	FVector TraceDirection = GetActorForwardVector() * AttackRange;
	FVector CenterVector = GetActorLocation() + TraceDirection * .5f;
	FQuat TraceQuat = FRotationMatrix::MakeFromZ(TraceDirection).ToQuat();
	FCollisionQueryParams Params(NAME_None, false, this);
	TArray<FOverlapResult> OverlapResults;
	bool bResult = GetWorld()->OverlapMultiByChannel(
		OverlapResults,
		CenterVector,
		TraceQuat,
		ECollisionChannel::ECC_GameTraceChannel2,
		FCollisionShape::MakeCapsule(AttackCollisionScale * 1.5f, AttackRange * .5f),
		Params
	);

	if (bResult) {
		for (FOverlapResult OverlapResult : OverlapResults) {
			APOECharacter_Base* DetectCharacter = Cast<APOECharacter_Base>(OverlapResult.GetActor());
			if (DetectCharacter == nullptr) continue;

			AController* PlayerController = DetectCharacter->GetInstigatorController();
			if (PlayerController->IsPlayerController()) {
				FDamageEvent DamageEvent;
				DetectCharacter->TakeDamage(CharacterStatus->AttackValue, DamageEvent, PlayerController, this);
#if ENABLE_DRAW_DEBUG
				DrawDebugCapsule(GetWorld(), CenterVector, AttackRange * 0.5f, AttackCollisionScale * 1.5f, TraceQuat, FColor::Green, false, .1f);
#endif
				return;
			}
		}
	}
	else {
#if ENABLE_DRAW_DEBUG
		DrawDebugCapsule(GetWorld(), CenterVector, AttackRange * 0.5f, AttackCollisionScale * 1.5f, TraceQuat, FColor::Red, false, .1f);
#endif
	}
}

float APOEMonster_Base::GetAIDetectRange()
{
	return AIDetectDistance;
}
void APOEMonster_Base::Die()
{
	Super::Die();
	APOEMonsterAIController* MonsterAIController = Cast<APOEMonsterAIController>(GetController());
	if (MonsterAIController != nullptr) {
		MonsterAIController->StopAI();
	}
	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, FTimerDelegate::CreateLambda([this]() {
		this->SetActorHiddenInGame(true);
	}), 3.0f, false);
}
float APOEMonster_Base::GetAttackDelay()
{
	return DelayAfterAttack;
}
\
