// Fill out your copyright notice in the Description page of Project Settings.

#include "POENpcMenuWidget.h"
#include "Components/Button.h"
#include "POENpcCharacter.h"

void UPOENpcMenuWidget::NativeConstruct() {
	Super::NativeConstruct();

	TalkButton = Cast<UButton>(GetWidgetFromName(TEXT("Button_Talk")));
	if (TalkButton != nullptr) {
		TalkButton->OnClicked.AddDynamic(this, &UPOENpcMenuWidget::OnTalkButtonClick);
	}

	TradeButton = Cast<UButton>(GetWidgetFromName(TEXT("Button_Trade")));
	if (TradeButton != nullptr) {
		TradeButton->OnClicked.AddDynamic(this, &UPOENpcMenuWidget::OnTradeButtonClick);
	}

	CancelButton = Cast<UButton>(GetWidgetFromName(TEXT("Button_Cancel")));
	if (CancelButton != nullptr) {
		CancelButton->OnClicked.AddDynamic(this, &UPOENpcMenuWidget::OnCancelButtonClick);
	}
}

void UPOENpcMenuWidget::OnCancelButtonClick()
{
	auto POENpcCharacter = Cast<APOENpcCharacter>(GetOwningPlayerPawn());
	CHECKRETURN(POENpcCharacter == nullptr);

	POENpcCharacter->OnShowMenuBar(false);
}

void UPOENpcMenuWidget::OnTradeButtonClick()
{
	OnTrade.Broadcast();
}

void UPOENpcMenuWidget::OnTalkButtonClick()
{
	OnTalk.Broadcast();
}

void UPOENpcMenuWidget::BeginDestroy()
{
	OnTalk.Clear();
	OnTrade.Clear();
}
