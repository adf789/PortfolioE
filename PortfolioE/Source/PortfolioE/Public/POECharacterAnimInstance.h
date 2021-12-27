// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "PortfolioE.h"
#include "Animation/AnimInstance.h"
#include "POECharacterAnimInstance.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnAttackCheckDelegate);
DECLARE_DELEGATE(FOnAttackSway);
/**
 * 
 */
UCLASS()
class PORTFOLIOE_API UPOECharacterAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

#pragma region 함수
public:
	UPOECharacterAnimInstance();

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	void PlayMeleeAttack();
	void JumpToAttackMeleeCombo(int32 NewSection);
	void PlayDash();
	void PlayCastMagic();

	FOnAttackCheckDelegate OnNextComboCheck;
	FOnAttackCheckDelegate OnAttackCollision;
	FOnAttackSway OnAttackSway;

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
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	float PawnSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	bool IsDead;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	UAnimMontage* MeleeAttackMontage;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	UAnimMontage* DashMontage;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	UAnimMontage* CastMontage;
#pragma endregion

};
