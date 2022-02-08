// Fill out your copyright notice in the Description page of Project Settings.

#include "POEPlayerHUDWidget.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/ProgressBar.h"
#include "POECharacterStat.h"
#include "POECharacter.h"
#include "MyInventoryComponent.h"
#include "POEGameInstance.h"
#include "InventoryItem_Equipment.h"
#include "Styling/SlateBrush.h"

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

	UMyInventoryComponent* Inventory = Character->Inventory;

	UTexture2D* ActiveItemTexture = nullptr;
	UTexture2D* PassiveItemTexture = nullptr;

	if (Inventory->GetEquippedActiveItem() != nullptr) {
		ActiveItemTexture = GameInstance->GetItemTextureForId(Inventory->GetEquippedActiveItem()->GetItemId());
		ActiveSlotImage->WidgetStyle.BackgroundImage.SetResourceObject(ActiveItemTexture);
		ActiveSlotImage->SetVisibility(ESlateVisibility::Visible);
	}
	else {
		ActiveSlotImage->SetVisibility(ESlateVisibility::Hidden);
	}
	
	if (Inventory->GetEquippedPassiveItem() != nullptr) {
		PassiveItemTexture = GameInstance->GetItemTextureForId(Inventory->GetEquippedPassiveItem()->GetItemId());
		PassiveSlotImage->SetBrushFromTexture(PassiveItemTexture);
		PassiveSlotImage->SetVisibility(ESlateVisibility::Visible);
	}
	else {
		PassiveSlotImage->SetVisibility(ESlateVisibility::Hidden);
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
	CHECKRETURN(MpBar == nullptr);

	HpBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("PB_HpBar")));
	CHECKRETURN(HpBar == nullptr);

	HpValueText = Cast<UTextBlock>(GetWidgetFromName(TEXT("Text_HpValue")));
	CHECKRETURN(HpValueText == nullptr);

	MpValueText = Cast<UTextBlock>(GetWidgetFromName(TEXT("Text_MpValue")));
	CHECKRETURN(MpValueText == nullptr);

	ActiveSlotImage = Cast<UProgressBar>(GetWidgetFromName(TEXT("Image_QuickActiveSlot")));
	CHECKRETURN(ActiveSlotImage == nullptr);

	PassiveSlotImage = Cast<UImage>(GetWidgetFromName(TEXT("Image_QuickPassiveSlot")));
	CHECKRETURN(PassiveSlotImage == nullptr);

	DashSlotImage = Cast<UProgressBar>(GetWidgetFromName(TEXT("PB_DashCoolTime")));
	CHECKRETURN(DashSlotImage == nullptr);

	UpdateHpBar();
	UpdateMpBar();
	InitQuickSlotView();
}

void UPOEPlayerHUDWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime) {
	Super::NativeTick(MyGeometry, InDeltaTime);

	CheckCoolTime(InDeltaTime);
}

void UPOEPlayerHUDWidget::CheckCoolTime(float DeltaTime) {
	bool ChangedCoolTime = false;

	if (ActiveSlotCoolTime > 0) {
		ActiveSlotCoolTime -= DeltaTime;

		if (ActiveSlotCoolTime <= 0 && OnEndActiveCoolTime.IsBound()) OnEndActiveCoolTime.Execute();
		ChangedCoolTime = true;
	}

	if (DashSlotCoolTime > 0) {
		DashSlotCoolTime -= DeltaTime;

		if (DashSlotCoolTime <= 0 && OnEndDashCoolTime.IsBound()) OnEndDashCoolTime.Execute();
		ChangedCoolTime = true;
	}

	if (ChangedCoolTime) {
		OnChangedCoolTime();
	}
}

void UPOEPlayerHUDWidget::OnChangedCoolTime() {
	ActiveSlotImage->SetPercent(ActiveSlotCoolTime / ActiveSlotEndCoolTime);

	if (DashSlotCoolTime > 0) {
		DashSlotImage->SetVisibility(ESlateVisibility::Visible);
		DashSlotImage->SetPercent(DashSlotCoolTime / DashSlotEndCoolTime);
	}
	else {
		DashSlotImage->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UPOEPlayerHUDWidget::SetTimerActiveSlot(float EndTime) {
	ActiveSlotCoolTime = EndTime;
	ActiveSlotEndCoolTime = EndTime;
}

void UPOEPlayerHUDWidget::SetTimerDashSlot(float EndTime) {
	DashSlotCoolTime = EndTime;
	DashSlotEndCoolTime = EndTime;
}
