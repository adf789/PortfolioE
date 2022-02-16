// Fill out your copyright notice in the Description page of Project Settings.

#include "POEMonster_Lottery.h"
#include "POEMonsterAIController.h"
#include "BehaviorTree/BlackboardComponent.h"


void APOEMonster_Lottery::PossessedBy(AController* NewController) {
	Super::PossessedBy(NewController);

	AIController = Cast<APOEMonsterAIController>(NewController);
	CHECKRETURN(AIController == nullptr);

	AIController->OnPostRunAI.AddUObject(this, &APOEMonster_Lottery::BindFunctionForRunAI);
}

void APOEMonster_Lottery::BindFunctionForRunAI() {
	CHECKRETURN(AIController == nullptr);

	AIController->GetBlackboardComponent()->SetValueAsBool(APOEMonsterAIController::BBKEY_IsRunMonster, true);
}

float APOEMonster_Lottery::TakeDamage(float DamageAmount, FDamageEvent const & DamageEvent, AController * EventInstigator, AActor * DamageCauser)
{
	GetWorld()->GetTimerManager().ClearTimer(SpeedUpTimerHandle);
	GetCharacterMovement()->MaxWalkSpeed = CharacterStatus->MoveSpeedValue * 2.0f;

	GetWorld()->GetTimerManager().SetTimer(SpeedUpTimerHandle, FTimerDelegate::CreateLambda([&]() {
		this->GetCharacterMovement()->MaxWalkSpeed = this->CharacterStatus->MoveSpeedValue;
		}), 3.0f, false);
	return Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
}
