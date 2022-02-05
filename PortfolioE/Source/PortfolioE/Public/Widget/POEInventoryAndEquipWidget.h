// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "PortfolioE.h"
#include "Blueprint/UserWidget.h"
#include "POEInventoryAndEquipWidget.generated.h"

/**
 * 
 */
UCLASS()
class PORTFOLIOE_API UPOEInventoryAndEquipWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION()
	void InitInventoryView(class UMyInventoryComponent* Inventory);
	void SetEquipItemView(class UTexture2D* ItemImage);

	UFUNCTION()
	void OnUnEuquipButtonClick();

protected:
	virtual void NativeConstruct() override;	

protected:
	UPROPERTY()
		class UWrapBox* InventoyBox;

	UPROPERTY()
		class UImage* EquippedSlot;

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	TSubclassOf<class UPOEItemSlotWidget> ItemSlotWidgetClass;	
};
