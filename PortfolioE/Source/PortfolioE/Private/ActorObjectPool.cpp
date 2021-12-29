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
	TEST_LOG_WITH_VAR("ActorObjectPool count : %d", EffectPooling.Num());
	
	while (true) {
		if (EffectPooling.Num() == 0) break;

		APoolingActor* Object = EffectPooling.Pop();

		if (!::IsValid(Object)) {
			TEST_LOG("This is invalid");
		}
		else if (Object != nullptr && !Object->IsActive()) {
			UE_LOG(POE, Warning, TEXT("actor : %s"), *Object->GetName());
			return Object;
		}
		else if (Object == nullptr) {
			UE_LOG(POE, Warning, TEXT("actor is null"), *Object->GetName());
		}
		else if (!Object->IsActive()) {
			UE_LOG(POE, Warning, TEXT("%s is not active"), *Object->GetName());
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
