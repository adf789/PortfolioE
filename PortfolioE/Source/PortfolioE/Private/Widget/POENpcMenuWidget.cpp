// Fill out your copyright notice in the Description page of Project Settings.

#include "POENpcMenuWidget.h"
#include "Components/Button.h"
#include "POEPlayerController.h"

void UPOENpcMenuWidget::NativeConstruct() {
	Super::NativeConstruct();

	TalkButton = Cast<UButton>(GetWidgetFromName(TEXT("Button_Talk")));
	if (TalkButton != nullptr) {
		TalkButton->OnClicked.AddDynamic(this, &UPOENpcMenuWidget::OnTalkButtonClick);
	}

	ActionButton = Cast<UButton>(GetWidgetFromName(TEXT("Button_Action")));
	if (ActionButton != nullptr) {
		ActionButton->OnClicked.AddDynamic(this, &UPOENpcMenuWidget::OnActionButtonClick);
	}

	CancelButton = Cast<UButton>(GetWidgetFromName(TEXT("Button_Cancel")));
	if (CancelButton != nullptr) {
		CancelButton->OnClicked.AddDynamic(this, &UPOENpcMenuWidget::OnCancelButtonClick);
	}
}

void UPOENpcMenuWidget::OnCancelButtonClick()
{
	/*auto POEPlayerController = Cast<APOEPlayerController>(this->GetOwningPlayer());
	CHECKRETURN(POEPlayerController == nullptr);

	POEPlayerController->HideNpcMenuWidget();*/
	OnCancel.Broadcast();
}

void UPOENpcMenuWidget::OnActionButtonClick()
{
	OnAction.Broadcast();
}

void UPOENpcMenuWidget::OnTalkButtonClick()
{
	OnTalk.Broadcast();
}

void UPOENpcMenuWidget::BeginDestroy()
{
	Super::BeginDestroy();
	OnTalk.Clear();
	OnAction.Clear();
	OnCancel.Clear();
}
