// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "PortfolioE.h"
#include "Blueprint/UserWidget.h"
#include "POEMessageBoxWidget.generated.h"

/**
 * 
 */
UCLASS()
class PORTFOLIOE_API UPOEMessageBoxWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION()
	void ClosePanel();
	void SetText(FString Text);
	
protected:
	virtual void NativeConstruct() override;
	
	UPROPERTY()
	class UTextBlock* MessageText;
};
