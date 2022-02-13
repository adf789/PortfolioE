// Fill out your copyright notice in the Description page of Project Settings.

#include "ActorObjectPool.h"
#include "POEMonster_Base.h"

ActorObjectPool::ActorObjectPool()
{
}

ActorObjectPool::~ActorObjectPool()
{
	
}

void ActorObjectPool::AddObject(AActor * poolingObject)
{
	CreatedActors.Push(poolingObject);
}

void ActorObjectPool::AddMonster(APOEMonster_Base * poolingObject)
{
	CreatedMonsters.Push(poolingObject);
}

AActor * ActorObjectPool::GetUnUseObject()
{
	while (true) {
		if (CreatedActors.Num() == 0) break;

		AActor* Object = CreatedActors.Pop();

		if (!::IsValid(Object) || Object == nullptr || !Object->bHidden) {
			continue;
		}
		else if (Object != nullptr && Object->bHidden) {
			return Object;
		}
	}

	return nullptr;
}

APOEMonster_Base * ActorObjectPool::GetUnUseMonster(int32 MonsterId)
{
	if (CreatedMonsters.Num() == 0) return nullptr;

	for (int i = 0; i < CreatedMonsters.Num(); i++) {
		APOEMonster_Base* Monster = CreatedMonsters[i];

		if (!::IsValid(Monster) || Monster == nullptr || !Monster->bHidden) {
			continue;
		}
		else if (Monster != nullptr && Monster->bHidden && Monster->MonsterId == MonsterId) {
			return Monster;
		}
	}

	return nullptr;
}

void ActorObjectPool::ResetAllObjects()
{
	for (AActor* actor : CreatedActors) {
		actor->SetActorHiddenInGame(true);
	}
}

void ActorObjectPool::ResetAllMonsters()
{
	for (auto it = CreatedMonsters.CreateConstIterator(); it; it++) {
		if ((*it) == nullptr || (*it)->IsPendingKill()) {
			CreatedMonsters.RemoveAt(it.GetIndex());
			continue;
		}

		(*it)->SetActorHiddenInGame(true);
	}
}

int32 ActorObjectPool::GetObjectCount()
{
	return CreatedActors.Num();
}

int32 ActorObjectPool::GetMonsterCount()
{
	return CreatedMonsters.Num();
}
