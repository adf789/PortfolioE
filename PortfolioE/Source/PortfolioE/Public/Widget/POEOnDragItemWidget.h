// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "PortfolioE.h"
#include "Blueprint/UserWidget.h"
#include "POEOnDragItemWidget.generated.h"

/**
 * 
 */
UCLASS()
class PORTFOLIOE_API UPOEOnDragItemWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetItemData(class UInventoryItem_Base* Item);
	
protected:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	
private:
	void SetLocationAtCursor();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	class UTextBlock* ItemNameText;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	class UImage* ItemImage;

	class UInventoryItem_Base* TargetItem;
};
