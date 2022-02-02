// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "PortfolioE.h"
#include "Blueprint/UserWidget.h"
#include "POEStageStartWidget.generated.h"

/**
 * 
 */
UCLASS()
class PORTFOLIOE_API UPOEStageStartWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
		void OnCancel();

	UFUNCTION(BlueprintCallable)
		void OnStageStart();

protected:
	virtual void NativeConstruct() override;

protected:
	UPROPERTY()
	class UButton* startButton;

	UPROPERTY()
	class UButton* cancelButton;

};
