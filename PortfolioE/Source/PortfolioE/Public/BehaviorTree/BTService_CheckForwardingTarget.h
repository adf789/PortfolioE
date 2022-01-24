// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "PortfolioE.h"
#include "BehaviorTree/BTService.h"
#include "BTService_CheckForwardingTarget.generated.h"

/**
 * 
 */
UCLASS()
class PORTFOLIOE_API UBTService_CheckForwardingTarget : public UBTService
{
	GENERATED_BODY()
	
public:
	UBTService_CheckForwardingTarget();
	
protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
