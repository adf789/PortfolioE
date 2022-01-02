// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "PortfolioE.h"
#include "AnimInstance_Base.h"
#include "POECharacterAnimInstance.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnAttackCheckDelegate);
DECLARE_DELEGATE(FOnAttackSway);
/**
 * 
 */
UCLASS()
class PORTFOLIOE_API UPOECharacterAnimInstance : public UAnimInstance_Base
{
	GENERATED_BODY()

#pragma region 함수
public:
	UPOECharacterAnimInstance();

	void PlayAttackCombo(int32 Section);
	void PlayDash();
	void PlayCastMagic();

	FOnAttackCheckDelegate OnNextComboCheck;
	FOnAttackCheckDelegate OnAttackCollision;
	FOnAttackSway OnAttackSway;

protected:
	/*Define function to attack motion */
	virtual void PlayAttackMotion() override;

	/*Define function to hit motion with direction */
	virtual void PlayHitMotion(FVector Direction) override;

private:
	UFUNCTION()
	void AnimNotify_AttackHit();

	UFUNCTION()
	void AnimNotify_ContinueCombo();

	UFUNCTION()
	void AnimNotify_SwayMeleeAttack();

	FName GetMeleeAttackSectionName(int32 Section);
#pragma endregion

#pragma region 변수
private:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	UAnimMontage* MeleeAttackMontage;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	UAnimMontage* DashMontage;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	UAnimMontage* CastMontage;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	int32 MaxAttackComboIndex;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	int32 CurAttackComboIndex;
#pragma endregion

};
