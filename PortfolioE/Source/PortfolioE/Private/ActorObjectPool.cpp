// Fill out your copyright notice in the Description page of Project Settings.

#include "ActorObjectPool.h"

ActorObjectPool::ActorObjectPool()
{
}

ActorObjectPool::~ActorObjectPool()
{
}

ActorObjectPool& ActorObjectPool::GetInstance()
{
	static ActorObjectPool* _instance = new ActorObjectPool();
	return *_instance;
}

void ActorObjectPool::AddEffect(APoolingActor * poolingObject)
{
	EffectPooling.Add(poolingObject);
}

APoolingActor * ActorObjectPool::GetUnUseEffect()
{
	for (APoolingActor* Object : EffectPooling) {
		if (Object != nullptr && !Object->IsActive()) {
			return Object;
		}
	}

	return nullptr;
}

void ActorObjectPool::ResetAllEffects()
{
	for (APoolingActor* actor : EffectPooling) {
		actor->InActive();
	}
}

int32 ActorObjectPool::GetEffectCount()
{
	return EffectPooling.Num();
}
