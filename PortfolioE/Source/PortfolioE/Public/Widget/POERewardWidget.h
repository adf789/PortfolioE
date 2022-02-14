// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "PortfolioE.h"
#include "Blueprint/UserWidget.h"
#include "POERewardWidget.generated.h"

/**
 * 
 */
UCLASS()
class PORTFOLIOE_API UPOERewardWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void SetRewardItemList(TArray<class UInventoryItem_Base*> ItemList);
	void AddCoinInRewardList(int32 CoinCount);

protected:
	virtual void NativeConstruct();
	
private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	class UWrapBox* RewardListView;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	TSubclassOf<class UPOERewardItemWidget> RewardListItemClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	class UPOEGameInstance* GameInstance;
};
