// Fill out your copyright notice in the Description page of Project Settings.

#include "MonsterSpawner.h"
#include "POEGameInstance.h"
#include "POEMonster_Base.h"
#include "engine/AssetManager.h"
#include "ActorObjectPool.h"

// Sets default values
AMonsterSpawner::AMonsterSpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	SpawnLocation = CreateDefaultSubobject<UBoxComponent>(TEXT("SpawnLocation"));
	SpawnLocation->SetCollisionProfileName(TEXT("NoCollision"));

	RootComponent = SpawnLocation;
}

FVector AMonsterSpawner::GetSpawnLocation()
{
	return SpawnLocation->GetComponentLocation();
}

// Called when the game starts or when spawned
void AMonsterSpawner::BeginPlay()
{
	Super::BeginPlay();
}

