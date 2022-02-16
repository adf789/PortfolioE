// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "PortfolioE.h"
#include "AIController.h"
#include "POEMonsterAIController.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnPostRunAI);
/**
 * 
 */
UCLASS()
class PORTFOLIOE_API APOEMonsterAIController : public AAIController
{
	GENERATED_BODY()

public:
	APOEMonsterAIController();

	virtual void Possess(APawn* InPawn) override;
	virtual void Tick(float DeltaTime) override;

	void RunAI();
	void StopAI();

	FOnPostRunAI OnPostRunAI;
	
protected:
	virtual void LoadBehaviorTree();

	UPROPERTY()
	class UBehaviorTree* BTPattern;

	UPROPERTY()
	class UBlackboardData* BBDatas;

public:
	static const FName BBKEY_Target;
	static const FName BBKEY_TargetDirection;
	static const FName BBKEY_AttackDist;
	static const FName BBKEY_WaypointLocation;
	static const FName BBKEY_SetWaypointDirection;
	static const FName BBKEY_IsRunMonster;
};
