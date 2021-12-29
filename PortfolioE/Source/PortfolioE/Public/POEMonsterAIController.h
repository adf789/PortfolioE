// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "PortfolioE.h"
#include "AIController.h"
#include "POEMonsterAIController.generated.h"

/**
 * 
 */
UCLASS()
class PORTFOLIOE_API APOEMonsterAIController : public AAIController
{
	GENERATED_BODY()

public:
	APOEMonsterAIController();
	
private:
	UPROPERTY()
	class UBehaviorTree* BTPattern;

	UPROPERTY()
	class UBlackboardData* BBDatas;
	
};
