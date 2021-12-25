// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "POECharacterAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class PORTFOLIOE_API UPOECharacterAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	UPOECharacterAnimInstance();

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	
private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	float pawnSpeed;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	bool isDead;
};
