// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "PortfolioE.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "AnimNotifyState_CharacterAttack.generated.h"

/**
 * 
 */
UCLASS()
class PORTFOLIOE_API UAnimNotifyState_CharacterAttack : public UAnimNotifyState
{
	GENERATED_BODY()
	
private:
	virtual void NotifyTick(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float FrameDeltaTime) override;
	
	
};
