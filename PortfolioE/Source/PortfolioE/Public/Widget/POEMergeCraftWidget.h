// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "PortFolioE.h"
#include "Blueprint/UserWidget.h"
#include "POEMergeCraftWidget.generated.h"

/**
 * 
 */
UCLASS()
class PORTFOLIOE_API UPOEMergeCraftWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION()
	void SetBaseSlot(class UInventoryItem_Equipment* BaseItem);

	UFUNCTION()
	void UnSetBaseSlot();

	UFUNCTION()
	void SetMaterialSlot(class UInventoryItem_Equipment* MaterialItem);

	UFUNCTION()
	void UnSetMaterialSlot();

	UFUNCTION()
	void SetItemToSlot(class UInventoryItem_Equipment* Item);

	UFUNCTION()
	void StartMergeCraft();

	UFUNCTION()
	void UpdateUIForValues();

	UFUNCTION()
	void ClosePanel();

protected:
	virtual void NativeConstruct() override;

private:
	void UpdateTextForValues();
	void UpdateExpBarForValues();
	void UpdateInventoryUI();

	UPROPERTY()
	class UInventoryItem_Equipment* BaseItem;

	UPROPERTY()
	class UInventoryItem_Equipment* MaterialItem;

	UPROPERTY()
	class UImage* BaseSlotImage;

	UPROPERTY()
	class UImage* MaterialSlotImage;

	UPROPERTY()
	class UProgressBar* ExpBar;

	UPROPERTY()
	class UTextBlock* BaseInfoText;

	UPROPERTY()
	class UTextBlock* MaterialInfoText;

	UPROPERTY()
	class UTextBlock* AttackValueText;

	UPROPERTY()
	class UTextBlock* HpValueText;

	UPROPERTY()
	class UTextBlock* SpeedValueText;

	UPROPERTY()
	class UTextBlock* ExpPercentText;

	UPROPERTY()
	class UTextBlock* BaseItemNameText;

	UPROPERTY()
	class UTextBlock* MaterialItemNameText;

	UPROPERTY()
	class UTextBlock* ItemLevelText;

	UPROPERTY()
	class UTextBlock* LevelUpInfoText;

	UPROPERTY()
	class UPOEGameInstance* GameInstance;
};
