// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "PortfolioE.h"
#include "Engine/GameInstance.h"
#include "Engine/StreamableManager.h"
#include "POEGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class PORTFOLIOE_API UPOEGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
	
public:
	UPOEGameInstance();

	FStreamableManager StreamableManager;

	class ActorObjectPool* EffectPooling;
	class ActorObjectPool* DamageTextPooling;
	class UUIScreenInteraction* UIScreenInteraction;
};
