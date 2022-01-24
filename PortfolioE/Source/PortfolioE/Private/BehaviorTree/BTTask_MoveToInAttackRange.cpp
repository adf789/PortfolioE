// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_MoveToInAttackRange.h"
#include "POECharacter_Base.h"
#include "AIController.h"

UBTTask_MoveToInAttackRange::UBTTask_MoveToInAttackRange() {
	NodeName = TEXT("Move To Target In AttackRange");
}

EBTNodeResult::Type UBTTask_MoveToInAttackRange::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	APOECharacter_Base* ControllingPawn = Cast<APOECharacter_Base>(OwnerComp.GetAIOwner()->GetPawn());
	if (ControllingPawn != nullptr) AcceptableRadius = ControllingPawn->GetAttackDistance() * .3f;
	return Super::ExecuteTask(OwnerComp, NodeMemory);
}
