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

	void AddObject(class APoolingActor* poolingObject);
	class APoolingActor* GetUnUseObject();
	void ResetAllObjects();
	int32 GetObjectCount();
	   
protected:
	TArray<class APoolingActor*> EffectPooling;

};
