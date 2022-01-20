// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "PortfolioE.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_AroundTo.generated.h"

/**
 * 
 */
UCLASS()
class PORTFOLIOE_API UBTTask_AroundTo : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBTTask_AroundTo();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

protected:
	UPROPERTY(EditAnywhere, Category = Blackboard)
	struct FBlackboardKeySelector BlackboardKey;

	UPROPERTY(VisibleAnywhere, Category = Blackboard)
	FVector FinalToLocation;

	UPROPERTY(VisibleAnywhere, Category = Blackboard)
	bool IsSetDistance;

	UPROPERTY(EditAnywhere, Category = Blackboard)
	float MaxMovableRadius;
};
