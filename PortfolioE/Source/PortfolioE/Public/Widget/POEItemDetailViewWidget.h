// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "PortfolioE.h"
#include "Blueprint/UserWidget.h"
#include "POEItemDetailViewWidget.generated.h"

/**
 * 
 */
UCLASS()
class PORTFOLIOE_API UPOEItemDetailViewWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetItemData(class UInventoryItem_Base* Item);

	UFUNCTION()
	void InitView();

	UFUNCTION()
	void SetAttackValueText(int32 Value);

	UFUNCTION()
	void SetHpValueText(int32 Value);

	UFUNCTION()
	void SetSpeedValueText(int32 Value);

	UFUNCTION()
	void SetLocationAtCursor();

protected:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	class UInventoryItem_Base* ItemData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	class UImage* ItemImage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	class UTextBlock* AttackValueText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	class UTextBlock* HpValueText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	class UTextBlock* SpeedValueText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	class UTextBlock* ItemTypeText;
};
