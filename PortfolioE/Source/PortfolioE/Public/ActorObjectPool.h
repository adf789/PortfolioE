// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "PortfolioE.h"
#include "PoolingActor.h"

/**
 * 
 */
class PORTFOLIOE_API ActorObjectPool
{
private:
	ActorObjectPool();
	~ActorObjectPool();

public:
	static ActorObjectPool& GetInstance();

	void AddEffect(APoolingActor* poolingObject);
	APoolingActor* GetUnUseEffect();
	void ResetAllEffects();
	int32 GetEffectCount();
	   
protected:
	TArray<class APoolingActor*> EffectPooling;

};
