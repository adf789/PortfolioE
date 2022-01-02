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

protected:

	
private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Meta = (AllowPrivateAccess = true))
	UParticleSystemComponent* ParticleSystemComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Meta = (AllowPrivateAccess = true))
	USphereComponent* DamageCollision;
};
