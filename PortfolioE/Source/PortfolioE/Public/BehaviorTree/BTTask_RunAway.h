// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "PortfolioE.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_RunAway.generated.h"

/**
 * 
 */
UCLASS()
class PORTFOLIOE_API UBTTask_RunAway : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
private:
	FNavLocation RandPoint;
	bool IsInit;
};
