// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "PortfolioE.h"
#include "Blueprint/UserWidget.h"
#include "POELotteryWidget.generated.h"

/**
 * 
 */
UCLASS()
class PORTFOLIOE_API UPOELotteryWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION()
	void StartLottery();

	UFUNCTION()
	void ClosePanel();

protected:
	virtual void NativeConstruct() override;

private:
	UPROPERTY()
		class UTextBlock* HaveCoinText;
	
};
