// Fill out your copyright notice in the Description page of Project Settings.

#include "EffectDamageActor.h"
#include "ActorObjectPool.h"
#include "POEGameInstance.h"

AEffectDamageActor::AEffectDamageActor() {
	PrimaryActorTick.bCanEverTick = true;
	DamageCollision = CreateDefaultSubobject<USphereComponent>(TEXT("DamageArea"));
	ParticleSystemComponent = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ParticleEffect"));
	DamageCollision->SetCollisionProfileName(TEXT("OverlapAll"));

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

	InitValues();
}

void AEffectDamageActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (CheckInActiveOnCondition()) {
		InActive();
	}
	else {
		SetNextValue(DeltaTime);
	}
}

void AEffectDamageActor::SetTimer(float Time)
{
	InitValues();

	IsSetTimer = true;
	EndTime = Time;
}

void AEffectDamageActor::SetDistance(float Distance)
{
	InitValues();

	IsSetDistance = true;
	EndDistance = Distance;
	StartLocation = GetActorLocation();
}

void AEffectDamageActor::SetDirection(FVector Direction)
{
	ForwardVector = Direction;
}

void AEffectDamageActor::InitValues()
{
	IsSetTimer = false;
	IsSetDistance = false;
	EndTime = .0f;
	PassingTime = .0f;
	EndDistance = .0f;
}

void AEffectDamageActor::BeginPlay()
{
	Super::BeginPlay();
	InitValues();
}

bool AEffectDamageActor::CheckInActiveOnCondition()
{
	if (IsSetTimer && PassingTime >= EndTime) return true;
	else if (IsSetDistance && FVector::Distance(StartLocation, GetActorLocation()) >= EndDistance) return true;
	else return false;
}

void AEffectDamageActor::SetNextValue(float DeltaTime)
{
	if (IsSetTimer) {
		PassingTime += DeltaTime;
	}
	else if (IsSetDistance) {
		SetActorLocation(GetActorLocation() + ForwardVector * 600 * DeltaTime);
	}
}
