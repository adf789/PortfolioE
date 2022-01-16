// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "PortfolioE.h"
#include "AnimInstance_Base.h"
#include "POEGruxAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class PORTFOLIOE_API UPOEGruxAnimInstance : public UAnimInstance_Base
{
	GENERATED_BODY()

public:
	UPOEGruxAnimInstance();

protected:
	/*Define function to attack motion */
	virtual void PlayAttackMotion() override;

	/*Define function to hit motion with direction */
	virtual void PlayHitMotion(FVector Direction) override;

private:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Anim, Meta = (AllowPrivateAccess = true))
	class UAnimMontage* GruxMeleeAttack1Anim;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Anim, Meta = (AllowPrivateAccess = true))
	class UAnimMontage* GruxMeleeAttack2Anim;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Anim, Meta = (AllowPrivateAccess = true))
	class UAnimMontage* GruxHitAnim;

	static const int8 MaxMeleeAttackIndex;
	int8 PlayedAttackAnimIndex;
};
