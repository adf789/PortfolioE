// Fill out your copyright notice in the Description page of Project Settings.

#include "ActorObjectPool.h"
#include "PoolingActor.h"

ActorObjectPool::ActorObjectPool()
{
}

ActorObjectPool::~ActorObjectPool()
{
	
}

void ActorObjectPool::AddObject(APoolingActor * poolingObject)
{
	EffectPooling.Push(poolingObject);
}

APoolingActor * ActorObjectPool::GetUnUseObject()
{
	while (true) {
		if (EffectPooling.Num() == 0) break;

		APoolingActor* Object = EffectPooling.Pop();

		if (!::IsValid(Object) || Object == nullptr || Object->IsActive()) {
			continue;
		}
		else if (Object != nullptr && !Object->IsActive()) {
			return Object;
		}
	}

	return nullptr;
}

void ActorObjectPool::ResetAllObjects()
{
	for (APoolingActor* actor : EffectPooling) {
		actor->InActive();
	}
}

int32 ActorObjectPool::GetObjectCount()
{
	return EffectPooling.Num();
}
