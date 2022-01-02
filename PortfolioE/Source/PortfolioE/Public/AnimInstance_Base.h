// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "PortfolioE.h"
#include "Animation/AnimInstance.h"
#include "AnimInstance_Base.generated.h"

/**
 * 
 */
UCLASS()
class PORTFOLIOE_API UAnimInstance_Base : public UAnimInstance
{
	GENERATED_BODY()

public:
	UAnimInstance_Base();
	
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	void PlayAttack();
	void PlayHit(FVector Direction);

protected:
	/*Define function to attack motion */
	virtual void PlayAttackMotion();
	
	/*Define function to hit motion with direction */
	virtual void PlayHitMotion(FVector Direction);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Pawn)
	float PawnSpeed;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Pawn)
	bool IsDead;
};
