// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AnimInstance_Base.h"
#include "POENormalMonster_Anim.generated.h"

/**
 * 
 */
UCLASS()
class PORTFOLIOE_API UPOENormalMonster_Anim : public UAnimInstance_Base
{
	GENERATED_BODY()
	
public:
	UPOENormalMonster_Anim();

protected:
	/*Define function to attack motion */
	virtual void PlayAttackMotion() override;

	/*Define function to hit motion with direction */
	virtual void PlayHitMotion(FVector Direction) override;

private:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Anim, Meta = (AllowPrivateAccess = true))
	class UAnimMontage* MeleeAttackAnim;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Anim, Meta = (AllowPrivateAccess = true))
	class UAnimMontage* HitAnim;
	
	
};
