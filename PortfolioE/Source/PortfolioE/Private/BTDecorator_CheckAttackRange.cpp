// Fill out your copyright notice in the Description page of Project Settings.

#include "BTDecorator_CheckAttackRange.h"
#include "POECharacter.h"
#include "POEMonster.h"
#include "POEMonsterAIController.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTDecorator_CheckAttackRange::UBTDecorator_CheckAttackRange() {
	NodeName = TEXT("CheckAttackRange");
}

bool UBTDecorator_CheckAttackRange::CalculateRawConditionValue(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory) const
{
	APOEMonster* ControllingPawn = Cast<APOEMonster>(OwnerComp.GetOwner());
	if (ControllingPawn == nullptr) return false;

	APOECharacter* Target = Cast<APOECharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(APOEMonsterAIController::BBKEY_Target));
	if (Target == nullptr) return false;

	return (ControllingPawn->GetDistanceTo(Target) <= ControllingPawn->GetAttackRange());
}
