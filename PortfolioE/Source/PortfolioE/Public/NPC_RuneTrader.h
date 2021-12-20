// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "POENpcCharacter.h"
#include "NPC_RuneTrader.generated.h"

/**
 * 
 */
UCLASS()
class PORTFOLIOE_API ANPC_RuneTrader : public APOENpcCharacter
{
	GENERATED_BODY()

public:
	ANPC_RuneTrader();
	
	virtual void PostInitializeComponents() override;

	UFUNCTION()
	void OnTalkButtonClicked();

	UFUNCTION()
	void OnTradeButtonClicked();
	
};
