// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "PortfolioE.h"

/**
 * 
 */
class PORTFOLIOE_API ActorObjectPool
{
public:
	ActorObjectPool();
	~ActorObjectPool();

	void AddObject(AActor* poolingObject);
	void AddMonster(class APOEMonster_Base* poolingObject);
	AActor* GetUnUseObject();
	class APOEMonster_Base* GetUnUseMonster(int32 MonsterId);
	void ResetAllObjects();
	void ResetAllMonsters();
	int32 GetObjectCount();
	int32 GetMonsterCount();
	   
protected:
	UPROPERTY()
	TArray<AActor*> CreatedActors;

	UPROPERTY()
	TArray<class APOEMonster_Base*> CreatedMonsters;
};
