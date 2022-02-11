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

	void AddRandonStatToItem(class UInventoryItem_Equipment* EquipmentItem);

protected:
	virtual void NativeConstruct() override;
	class UInventoryItem_Base* CreateLotteryItem();
	void ShowMessagePanel(FString Text);

private:
	int32 GetItemIdForRandom();

	UPROPERTY()
		class UTextBlock* HaveCoinText;
	
};
