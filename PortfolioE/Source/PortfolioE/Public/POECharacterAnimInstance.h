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

	virtual void NativeUpdateAnimation(float deltaSeconds) override;

	void PlayMeleeAttack();
	void JumpToAttackMeleeCombo(int32 newSection);

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

	FName GetMeleeAttackSectionName(int32 section);
#pragma endregion

#pragma region 변수
private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
		float pawnSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
		bool isDead;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		UAnimMontage* meleeAttackMontage;
#pragma endregion

};
