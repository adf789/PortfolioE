// Fill out your copyright notice in the Description page of Project Settings.

#include "BTDecorator_CheckAttackRange.h"
#include "POECharacter.h"
#include "POECharacter_Base.h"
#include "POEMonsterAIController.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTDecorator_CheckAttackRange::UBTDecorator_CheckAttackRange() {
	NodeName = TEXT("CheckAttackRange");
}

bool UBTDecorator_CheckAttackRange::CalculateRawConditionValue(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory) const
{
	bool bResult = Super::CalculateRawConditionValue(OwnerComp, NodeMemory);
	APOECharacter_Base* ControllingPawn = Cast<APOECharacter_Base>(OwnerComp.GetAIOwner()->GetPawn());
	CHECKRETURN(ControllingPawn == nullptr, false);

	APOECharacter* Target = Cast<APOECharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(APOEMonsterAIController::BBKEY_Target));
	CHECKRETURN(Target == nullptr, false);

	bResult = ControllingPawn->GetDistanceTo(Target) <= ControllingPawn->GetAttackRange();
	return bResult;
}
