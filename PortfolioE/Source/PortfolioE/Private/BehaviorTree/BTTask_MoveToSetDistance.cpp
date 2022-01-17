// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_MoveToSetDistance.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "POEMonsterAIController.h"

EBTNodeResult::Type UBTTask_MoveToSetDistance::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	AcceptableRadius = OwnerComp.GetBlackboardComponent()->GetValueAsFloat(APOEMonsterAIController::BBKEY_AttackDist) * 0.5f;
	return Super::ExecuteTask(OwnerComp, NodeMemory);
}
