// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_AroundTo.h"

UBTTask_AroundTo::UBTTask_AroundTo() {

}

EBTNodeResult::Type UBTTask_AroundTo::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	EBTNodeResult::Type result = Super::ExecuteTask(OwnerComp, NodeMemory);

	return result;
}
