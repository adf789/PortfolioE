// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "PortfolioE.h"
#include "PoolingActor.h"
#include "EffectDamageActor.generated.h"

/**
 * 
 */
UCLASS()
class PORTFOLIOE_API AEffectDamageActor : public APoolingActor
{
	GENERATED_BODY()

public:
	AEffectDamageActor();
	void SetParticleSystem(UParticleSystem* ParticleData);
	virtual void Active() override;
	virtual void InActive() override;
	virtual void Tick(float DeltaTime) override;
	
	void SetTimer(float Time);
	void SetDistance(float Distance);
	void SetDirection(FVector Direction);
	void InitValues();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	
private:
	UFUNCTION()
	bool CheckInActiveOnCondition();

	UFUNCTION()
	void SetNextValue(float DeltaTime);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Meta = (AllowPrivateAccess = true))
	UParticleSystemComponent* ParticleSystemComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Meta = (AllowPrivateAccess = true))
	USphereComponent* DamageCollision;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Meta = (AllowPrivateAccess = true))
	bool IsSetTimer;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Meta = (AllowPrivateAccess = true))
	float EndTime;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Meta = (AllowPrivateAccess = true))
	float PassingTime;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Meta = (AllowPrivateAccess = true))
	bool IsSetDistance;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Meta = (AllowPrivateAccess = true))
	float EndDistance;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Meta = (AllowPrivateAccess = true))
	FVector StartLocation;

	FVector ForwardVector;
};
