// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "PortfolioE.h"
#include "Animation/AnimInstance.h"
#include "POEGruxAnimInstance.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnAnimEvent);
/**
 * 
 */
UCLASS()
class PORTFOLIOE_API UPOEGruxAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	UPOEGruxAnimInstance();

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	FOnAnimEvent OnSpawnEnd;

private:
	UFUNCTION()
	void AnimNotify_SpawnEnd();

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	bool IsSpawned;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	float PawnSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	bool IsDead;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	float RotationRate;
	
};
