// Fill out your copyright notice in the Description page of Project Settings.

#include "EffectDamageActor.h"
#include "ActorObjectPool.h"
#include "POEGameInstance.h"

AEffectDamageActor::AEffectDamageActor() : Super() {
	PrimaryActorTick.bCanEverTick = false;
	DamageCollision = CreateDefaultSubobject<USphereComponent>(TEXT("DamageArea"));
	ParticleSystemComponent = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ParticleEffect"));

	RootComponent = DamageCollision;
	ParticleSystemComponent->SetupAttachment(DamageCollision);
}

void AEffectDamageActor::SetParticleSystem(UParticleSystem * ParticleData)
{
	ParticleSystemComponent->SetTemplate(ParticleData);
}

void AEffectDamageActor::Active()
{
	Super::Active();
	ParticleSystemComponent->ActivateSystem(true);
}

void AEffectDamageActor::InActive()
{
	Super::InActive();
	UPOEGameInstance* POEGameInstance = Cast<UPOEGameInstance>(GetGameInstance());
	CHECKRETURN(POEGameInstance == nullptr);
	POEGameInstance->EffectPooling->AddObject(this);
}
