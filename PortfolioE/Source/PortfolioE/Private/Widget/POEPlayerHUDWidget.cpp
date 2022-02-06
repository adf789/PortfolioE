// Fill out your copyright notice in the Description page of Project Settings.

#include "POEPlayerHUDWidget.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "POECharacterStat.h"
#include "POECharacter.h"
#include "MyInventoryComponent.h"
#include "POEGameInstance.h"
#include "InventoryItem_Equipment.h"

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

void UPOEPlayerHUDWidget::InitQuickSlotView()
{
	APOECharacter* Character = Cast<APOECharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());
	CHECKRETURN(Character == nullptr);

	UPOEGameInstance* GameInstance = Cast<UPOEGameInstance>(GetWorld()->GetGameInstance());
	CHECKRETURN(GameInstance == nullptr);

	UTexture2D* ActiveItemTexture = nullptr;
	UTexture2D* PassiveItemTexture = nullptr;

	if (Character->Inventory->GetEquippedActiveItem() != nullptr) {
		ActiveItemTexture = GameInstance->GetItemTextureForId(Character->Inventory->GetEquippedActiveItem()->GetItemId());
	}
	ActiveSlotImage->SetBrushFromTexture(ActiveItemTexture);
	ActiveSlotImage->SetColorAndOpacity(ActiveItemTexture != nullptr ? FLinearColor::White : FLinearColor::Transparent);

	if (Character->Inventory->GetEquippedPassiveItem() != nullptr) {
		PassiveItemTexture = GameInstance->GetItemTextureForId(Character->Inventory->GetEquippedPassiveItem()->GetItemId());
	}
	PassiveSlotImage->SetBrushFromTexture(PassiveItemTexture);
	PassiveSlotImage->SetColorAndOpacity(PassiveItemTexture != nullptr ? FLinearColor::White : FLinearColor::Transparent);
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
	CHECKRETURN(MpBar == nullptr);

	HpBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("PB_HpBar")));
	CHECKRETURN(HpBar == nullptr);

	HpValueText = Cast<UTextBlock>(GetWidgetFromName(TEXT("Text_HpValue")));
	CHECKRETURN(HpValueText == nullptr);

	MpValueText = Cast<UTextBlock>(GetWidgetFromName(TEXT("Text_MpValue")));
	CHECKRETURN(MpValueText == nullptr);

	ActiveSlotImage = Cast<UImage>(GetWidgetFromName(TEXT("Image_QuickActiveSlot")));
	CHECKRETURN(ActiveSlotImage == nullptr);

	PassiveSlotImage = Cast<UImage>(GetWidgetFromName(TEXT("Image_QuickPassiveSlot")));
	CHECKRETURN(PassiveSlotImage == nullptr);

	UpdateHpBar();
	UpdateMpBar();
	InitQuickSlotView();
}
