// Fill out your copyright notice in the Description page of Project Settings.

#include "POEPlayerHUDWidget.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "POECharacterStat.h"

void UPOEPlayerHUDWidget::UpdateHpBar()
{
	if (CharacterStat != nullptr) {
		if (HpBar != nullptr) {
			HpBar->SetPercent(CharacterStat->GetHPRate());
		}

		if (HpValueText != nullptr) {
			HpValueText->SetText(FText::FromString(FString::Printf(TEXT("%.0f / %.0f"), CharacterStat->CurrentHPValue, CharacterStat->MaxHPValue)));
		}
	}
}

void UPOEPlayerHUDWidget::UpdateMpBar() 
{
	if (CharacterStat != nullptr) {
		if (MpBar != nullptr) {
			MpBar->SetPercent(CharacterStat->GetMPRate());
		}

		if (MpValueText != nullptr) {
			MpValueText->SetText(FText::FromString(FString::Printf(TEXT("%.0f / %.0f"), CharacterStat->CurrentMPValue, CharacterStat->MaxMPValue)));
		}
	}
}

void UPOEPlayerHUDWidget::BindCharacterStat(UPOECharacterStat * CharacterStat)
{
	this->CharacterStat = CharacterStat;

	this->CharacterStat->OnChangeHPAction.AddUObject(this, &UPOEPlayerHUDWidget::UpdateHpBar);
	this->CharacterStat->OnChangeMPAction.AddUObject(this, &UPOEPlayerHUDWidget::UpdateMpBar);

	UpdateHpBar();
	UpdateMpBar();
}

void UPOEPlayerHUDWidget::NativeConstruct() {
	Super::NativeConstruct();

	MpBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("PB_MpBar")));
	HpBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("PB_HpBar")));
	HpValueText = Cast<UTextBlock>(GetWidgetFromName(TEXT("Text_HpValue")));
	MpValueText = Cast<UTextBlock>(GetWidgetFromName(TEXT("Text_MpValue")));

	UpdateHpBar();
	UpdateMpBar();
}
