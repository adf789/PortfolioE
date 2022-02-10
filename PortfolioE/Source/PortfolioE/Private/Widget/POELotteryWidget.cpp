// Fill out your copyright notice in the Description page of Project Settings.

#include "POELotteryWidget.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "POEGameInstance.h"

void UPOELotteryWidget::NativeConstruct() {
	Super::NativeConstruct();

	HaveCoinText = Cast<UTextBlock>(GetWidgetFromName(TEXT("Text_HaveCoin")));
	CHECKRETURN(HaveCoinText == nullptr);

	UPOEGameInstance* GameInstance = Cast<UPOEGameInstance>(GetWorld()->GetGameInstance());
	CHECKRETURN(GameInstance);
	HaveCoinText->SetText(FText::AsNumber(GameInstance->LotteryCoinCount));

	UButton* StartLotteryButton = Cast<UButton>(GetWidgetFromName(TEXT("Button_StartLottery")));
	CHECKRETURN(StartLotteryButton == nullptr);

	UButton* CloseButton = Cast<UButton>(GetWidgetFromName(TEXT("Button_Close")));
	CHECKRETURN(CloseButton == nullptr);

	StartLotteryButton->OnClicked.AddDynamic(this, &UPOEMergeCraftWidget::StartLottery);
	CloseButton->OnClicked.AddDynamic(this, &UPOEMergeCraftWidget::ClosePanel);
}

void UPOELotteryWidget::StartLottery() {

}

void UPOELotteryWidget::ClosePanel() {
	UPOEGameInstance* GameInstance = Cast<UPOEGameInstance>(GetWorld()->GetGameInstance());
	CHECKRETURN(GameInstance == nullptr);

	GameInstance->UIScreenInteraction->ClosePanel(EUIPanelName::MERGE_CRAFT);
}

