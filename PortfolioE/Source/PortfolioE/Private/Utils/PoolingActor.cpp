// Fill out your copyright notice in the Description page of Project Settings.

#include "PoolingActor.h"
#include "ActorObjectPool.h"


// Sets default values
APoolingActor::APoolingActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void APoolingActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APoolingActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APoolingActor::Active()
{
	SetActorHiddenInGame(false);
}

void APoolingActor::InActive()
{
	SetActorHiddenInGame(true);
}

bool APoolingActor::IsActive()
{
	return !this->bHidden;
}

