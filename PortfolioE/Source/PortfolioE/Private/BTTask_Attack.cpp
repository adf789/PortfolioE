// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_Attack.h"
#include "POEMonster.h"

EBTNodeResult::Type UBTTask_Attack::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	APOEMonster* Monster = Cast<APOEMonster>(OwnerComp.GetOwner());

	return EBTNodeResult::InProgress;
}

void UBTTask_Attack::TickTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory, float DeltaSeconds)
{
	if (false) {
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}
