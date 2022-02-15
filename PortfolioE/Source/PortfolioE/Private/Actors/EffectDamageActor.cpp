// Fill out your copyright notice in the Description page of Project Settings.

#include "EffectDamageActor.h"
#include "ActorObjectPool.h"
#include "POEGameInstance.h"
#include "POEMonster_Base.h"
#include "POECharacter.h"
#include "POECharacterStat.h"

AEffectDamageActor::AEffectDamageActor() {
	PrimaryActorTick.bCanEverTick = true;
	DamageCollision = CreateDefaultSubobject<USphereComponent>(TEXT("DamageArea"));
	ParticleSystemComponent = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ParticleEffect"));
	DamageCollision->SetCollisionProfileName(TEXT("OverlapAll"));
	DamageCollision->SetSphereRadius(90.0f);
	DamageCollision->OnComponentBeginOverlap.AddDynamic(this, &AEffectDamageActor::OnOverlapBegin);
	//DamageCollision->OnComponentHit.AddDynamic(this, &AEffectDamageActor::OnHitBegin);

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

void AEffectDamageActor::SetAttacker(APOECharacter* Attacker)
{
	this->Attacker = Attacker;
}

void AEffectDamageActor::OnOverlapBegin(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	APOEMonster_Base* Monster = Cast<APOEMonster_Base>(OtherActor);
	if (Monster != nullptr) {
		Monster->TakeDamage(GetRandAttackValue(), FDamageEvent(), Attacker->GetController(), Attacker);
	}
}

void AEffectDamageActor::OnHitBegin(UPrimitiveComponent * HitComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, FVector NormalImpulse, const FHitResult & Hit)
{
	APOEMonster_Base* Monster = Cast<APOEMonster_Base>(Hit.GetActor());
	if (OtherActor != nullptr) {
		TEST_LOG_WITH_VAR("Hit: %s", *OtherActor->GetName());
	}
	if (Monster != nullptr) {
		Monster->TakeDamage(GetRandAttackValue(), FDamageEvent(), Attacker->GetController(), Attacker);
	}
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

float AEffectDamageActor::GetRandAttackValue()
{
	CHECKRETURN(Attacker == nullptr, .0f);
	FRandomStream Random(FMath::Rand());
	float RandDamage = Random.FRandRange(0.3f, 0.7f);

	float AttackValue = Attacker->CharacterStatus->AttackValue * RandDamage;

	return AttackValue;
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
