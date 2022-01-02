// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "PortfolioE.h"
#include "AnimInstance_Base.h"
#include "POEGruxAnimInstance.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnAnimEvent);
/**
 * 
 */
UCLASS()
class PORTFOLIOE_API UPOEGruxAnimInstance : public UAnimInstance_Base
{
	GENERATED_BODY()

public:
	UPOEGruxAnimInstance();

	FOnAnimEvent OnSpawnEnd;
	FOnAnimEvent OnAttackEnd;

	FName GetMeleeAttackName(int8 Section);

protected:
	/*Define function to attack motion */
	virtual void PlayAttackMotion() override;

	/*Define function to hit motion with direction */
	virtual void PlayHitMotion(FVector Direction) override;

private:
	UFUNCTION()
	void AnimNotify_SpawnEnd();

	UFUNCTION()
	void AnimNotify_AttackEnd();

private:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Anim = Anim, Meta = (AllowPrivateAccess = true))
	class UAnimMontage* GruxMeleeAttackAnim;

	static const FString MeleeAttackNames[];
	static const int8 MaxMeleeAttackIndex;
	int8 CurMeleeAttackIndex;
};
