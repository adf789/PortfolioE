// Description
// 오브젝트 풀링을 하기위한 클래스

#include "ActorObjectPool.h"
#include "POEMonster_Base.h"

ActorObjectPool::ActorObjectPool()
{
}

ActorObjectPool::~ActorObjectPool()
{
	CreatedActors.Reset();
	CreatedMonsters.Reset();
}

void ActorObjectPool::AddObject(AActor * poolingObject)
{
	CreatedActors.Push(poolingObject);
}

void ActorObjectPool::AddMonster(APOEMonster_Base * poolingObject)
{
	if (poolingObject == nullptr) return;
	
	if (!CreatedMonsters.Contains(poolingObject->MonsterId)) CreatedMonsters.Add(poolingObject->MonsterId, TArray<APOEMonster_Base*>());

	for (int i = 0; i < CreatedMonsters[poolingObject->MonsterId].Num(); i++) {
		if (CreatedMonsters[poolingObject->MonsterId][i] == poolingObject) {
			return;
		}
	}
	CreatedMonsters[poolingObject->MonsterId].Push(poolingObject);
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
	if (!CreatedMonsters.Contains(MonsterId)) return nullptr;
	else if (CreatedMonsters[MonsterId].Num() == 0) {
		return nullptr;
	}

	while (true) {
		if (CreatedMonsters[MonsterId].Num() == 0) break;

		APOEMonster_Base* Monster = CreatedMonsters[MonsterId].Pop();

		if (!::IsValid(Monster) || Monster == nullptr || !Monster->bHidden) {
			continue;
		}
		else if (Monster != nullptr && Monster->bHidden) {
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
	for (auto it = CreatedMonsters.CreateConstIterator(); it; ++it) {
		
		for (int i = 0; i < it->Value.Num(); i++) {
			it->Value[i]->SetActorHiddenInGame(true);
		}
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
