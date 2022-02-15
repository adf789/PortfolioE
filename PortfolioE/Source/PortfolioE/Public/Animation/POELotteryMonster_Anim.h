// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseClass/AnimInstance_Base.h"
#include "POELotteryMonster_Anim.generated.h"

/**
 * 
 */
UCLASS()
class PORTFOLIOE_API UPOELotteryMonster_Anim : public UAnimInstance_Base
{
	GENERATED_BODY()
	
public:
	UPOELotteryMonster_Anim();

protected:
	/*Define function to hit motion with direction */
	virtual void PlayHitMotion(FVector Direction) override;

private:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Anim, Meta = (AllowPrivateAccess = true))
		class UAnimMontage* HitAnim;
	
	
};
