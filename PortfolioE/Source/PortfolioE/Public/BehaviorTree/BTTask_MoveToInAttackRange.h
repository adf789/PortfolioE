// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "PortfolioE.h"
#include "BehaviorTree/Tasks/BTTask_MoveTo.h"
#include "BTTask_MoveToInAttackRange.generated.h"

/**
 * 
 */
UCLASS()
class PORTFOLIOE_API UBTTask_MoveToInAttackRange : public UBTTask_MoveTo
{
	GENERATED_BODY()
	
public:
	UBTTask_MoveToInAttackRange();
	
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

};
