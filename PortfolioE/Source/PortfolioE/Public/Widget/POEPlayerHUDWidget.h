// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "PortfolioE.h"
#include "Blueprint/UserWidget.h"
#include "POEPlayerHUDWidget.generated.h"

DECLARE_DELEGATE(FOnEndCoolTime);
/**
 * 
 */
UCLASS()
class PORTFOLIOE_API UPOEPlayerHUDWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

public:
	void UpdateHpBar();
	void UpdateMpBar();
	void InitQuickSlotView();

	void BindCharacterStat(class UPOECharacterStat* CharacterStat);
	
	void SetTimerActiveSlot(float EndTime);
	void SetTimerDashSlot(float EndTime);

	FOnEndCoolTime OnEndActiveCoolTime;
	FOnEndCoolTime OnEndDashCoolTime;

private:
	void CheckCoolTime(float DeltaTime);
	void OnChangedCoolTime();

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
	class UProgressBar* ActiveSlotImage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	class UImage* PassiveSlotImage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	class UProgressBar* DashSlotImage;
	
	UPROPERTY()
	class UPOECharacterStat* CharacterStat;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly)
	float ActiveSlotCoolTime;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly)
	float ActiveSlotEndCoolTime;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly)
	float DashSlotCoolTime;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly)
	float DashSlotEndCoolTime;
};
