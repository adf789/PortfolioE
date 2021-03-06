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
	void InitInventoryView();
	void SetActiveEquipImage(class UTexture2D* ItemImage);
	void SetPassiveEquipImage(class UTexture2D* ItemImage);

	UFUNCTION()
	void InitActiveEquipSlot(UInventoryItem_Equipment* EquippedItem);

	UFUNCTION()
	void OnActiveUnEuquipClick();

	UFUNCTION()
	void InitPassiveEquipSlot(UInventoryItem_Equipment* EquippedItem);

	UFUNCTION()
	void OnPassiveUnEuquipClick();

	UFUNCTION()
	void SetVisibilityEquipmentPanel(ESlateVisibility Visible);

protected:
	virtual void NativeConstruct() override;

protected:
	UPROPERTY()
	class UWrapBox* InventoyBox;

	UPROPERTY()
	class UImage* EquippedActiveImage;

	UPROPERTY()
	class UTextBlock* ActiveInfoText;

	UPROPERTY()
	class UImage* EquippedPassiveImage;

	UPROPERTY()
	class UTextBlock* PassiveInfoText;

	UPROPERTY()
	class UTextBlock* CharacterValueText_Attack;

	UPROPERTY()
	class UTextBlock* CharacterValueText_Hp;

	UPROPERTY()
	class UTextBlock* CharacterValueText_Speed;

	UPROPERTY()
	class UWidget* EquipmentPanel;

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	TSubclassOf<class UPOEItemSlotWidget> ItemSlotWidgetClass;
};
