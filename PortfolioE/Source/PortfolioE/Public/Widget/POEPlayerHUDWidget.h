// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "PortfolioE.h"
#include "Blueprint/UserWidget.h"
#include "POEPlayerHUDWidget.generated.h"

/**
 * 
 */
UCLASS()
class PORTFOLIOE_API UPOEPlayerHUDWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;

public:
	void UpdateHpBar();
	void UpdateMpBar();
	void InitQuickSlotView();

	void BindCharacterStat(class UPOECharacterStat* CharacterStat);
	

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	class UProgressBar* HpBar;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	class UProgressBar* MpBar;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	class UTextBlock* HpValueText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	class UTextBlock* MpValueText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	class UImage* ActiveSlotImage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	class UImage* PassiveSlotImage;
	
	UPROPERTY()
	class UPOECharacterStat* CharacterStat;
};
