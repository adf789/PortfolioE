// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "PortfolioE.h"
#include "Blueprint/UserWidget.h"
#include "POERewardItemWidget.generated.h"

/**
 * 
 */
UCLASS()
class PORTFOLIOE_API UPOERewardItemWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void InitItemView(class UInventoryItem_Base* ItemData);
	void InitCoinView(int32 CoinCount);
	
protected:
	virtual void NativeConstruct();
	
private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	class UImage* ItemImage;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	class UTextBlock* LevelText;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	class UTextBlock* ItemNameText;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	class UPOEGameInstance* GameInstance;
};
