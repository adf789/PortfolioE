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
	void SetMaterialSlot(class UInventoryItem_Equipment* BaseItem);

	UFUNCTION()
	void UnSetMaterialSlot();

	UFUNCTION()
	void StartMergeCraft();

protected:
	virtual void NativeConstruct() override;

private:
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
};
