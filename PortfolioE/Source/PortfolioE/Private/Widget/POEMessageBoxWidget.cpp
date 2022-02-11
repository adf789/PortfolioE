// Fill out your copyright notice in the Description page of Project Settings.

#include "POEMessageBoxWidget.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "POEGameInstance.h"

void UPOEMessageBoxWidget::ClosePanel() {
	UPOEGameInstance* GameInstance = Cast<UPOEGameInstance>(GetWorld()->GetGameInstance());
	CHECKRETURN(GameInstance == nullptr);

	GameInstance->UIScreenInteraction->ClosePanel(EUIPanelName::MESSAGE_BOX);
}

void UPOEMessageBoxWidget::SetText(FString Text) {
	MessageText->SetText(FText::FromString(Text));
}

void UPOEMessageBoxWidget::NativeConstruct() {
	Super::NativeConstruct();

	MessageText = Cast<UTextBlock>(GetWidgetFromName(TEXT("Content")));
	CHECKRETURN(MessageText == nullptr);

	UButton* CloseButton = Cast<UButton>(GetWidgetFromName(TEXT("Button_Close")));
	CHECKRETURN(CloseButton == nullptr);
	CloseButton->OnClicked.AddDynamic(this, &UPOEMessageBoxWidget::ClosePanel);
}