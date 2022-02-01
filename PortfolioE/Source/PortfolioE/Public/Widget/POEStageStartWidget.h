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

	UFUNCTION()
		void InitInventoryView(class UMyInventoryComponent* Inventory);

protected:
	virtual void NativeConstruct() override;

protected:
	UPROPERTY()
	class UButton* startButton;

	UPROPERTY()
	class UButton* cancelButton;

	UPROPERTY()
	class UWrapBox* inventoyBox; 

private:
	UPROPERTY()
	TSubclassOf<class UPOEItemSlotWidget> ItemSlotWidgetClass;
};
