// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "POECharacter_Base.h"
#include "POEMonster_Base.generated.h"

/**
 * 
 */
UCLASS()
class PORTFOLIOE_API APOEMonster_Base : public APOECharacter_Base
{
	GENERATED_BODY()
	
public:
	APOEMonster_Base();

	virtual void PostInitializeComponents() override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	virtual float GetAIDetectRange() override;
	virtual float GetAttackDistance() override;

protected:
	virtual void BeginPlay() override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = State, Meta = (AllowPrivateAccess = true))
	bool IsSpawned;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = State, Meta = (AllowPrivateAccess = true))
	float MaxSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = State, Meta = (AllowPrivateAccess = true))
	float AttackDistance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = State, Meta = (AllowPrivateAccess = true))
	float AIDetectDistance;
};
