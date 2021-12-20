// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "PortfolioE.h"
#include "Blueprint/UserWidget.h"
#include "POENpcMenuWidget.generated.h"


DECLARE_MULTICAST_DELEGATE(FOnButtonClicked);
/**
 * 
 */
UCLASS()
class PORTFOLIOE_API UPOENpcMenuWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;

	UFUNCTION()
	void OnCancelButtonClick();

	UFUNCTION()
	void OnTradeButtonClick();

	UFUNCTION()
	void OnTalkButtonClick();

public:
	FOnButtonClicked OnTalk;
	FOnButtonClicked OnTrade;

	virtual void BeginDestroy() override;
	
protected:
	UPROPERTY()
	class UButton* TalkButton;

	UPROPERTY()
	class UButton* TradeButton;

	UPROPERTY()
	class UButton* CancelButton;
};
