// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "PortfolioE.h"
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

	virtual void OnTalk() override;
	virtual void OnAction() override;
	virtual void OnCancel() override;
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = UI)
	TSubclassOf<class UPOEStageStartWidget> stageUIClass;

	UPROPERTY()
	class UPOEStageStartWidget* stageStartWidget;
};
