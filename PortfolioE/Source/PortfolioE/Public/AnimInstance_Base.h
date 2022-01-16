// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#define MaxBufferedCalculateCount 2

#include "PortfolioE.h"
#include "Animation/AnimInstance.h"
#include "AnimInstance_Base.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnAnimEvent);
/**
 * 
 */
UCLASS()
class PORTFOLIOE_API UAnimInstance_Base : public UAnimInstance
{
	GENERATED_BODY()

public:
	UAnimInstance_Base();
	
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	void PlayAttack();
	void PlayHit(FVector Direction);

	FOnAnimEvent OnSpawnEnd;
	FOnAnimEvent OnAttackEnd;

	virtual void BindCharacter(class APOECharacter_Base* Actor);

protected:
	/*Define function to attack motion */
	virtual void PlayAttackMotion();
	
	/*Define function to hit motion with direction */
	virtual void PlayHitMotion(FVector Direction);

private:
	UFUNCTION()
		void AnimNotify_SpawnEnd();

	UFUNCTION()
		void AnimNotify_AttackEnd();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Pawn)
	float PawnSpeed;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Pawn)
	float PawnRotateValue;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Pawn)
	bool IsDead;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Pawn)
	class APOECharacter_Base* CharacterActor;

private:
	UPROPERTY()
	FVector PrevForwardVector;

	int8 BufferedCalculateCount;
};
