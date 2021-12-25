// Fill out your copyright notice in the Description page of Project Settings.

#include "POEStageStartWidget.h"
#include "Components/Button.h"

void UPOEStageStartWidget::OnStageStart()
{
}

void UPOEStageStartWidget::OnCancel()
{
	RemoveFromParent();
	GetOwningPlayer()->SetPause(false);
}

void UPOEStageStartWidget::NativeConstruct() {
	Super::NativeConstruct();

	startButton = Cast<UButton>(GetWidgetFromName(TEXT("Btn_Start")));
	if (startButton != nullptr) {
		startButton->OnClicked.AddDynamic(this, &UPOEStageStartWidget::OnStageStart);
	}

	cancelButton = Cast<UButton>(GetWidgetFromName(TEXT("Btn_Cancel")));
	if (cancelButton != nullptr) {
		cancelButton->OnClicked.AddDynamic(this, &UPOEStageStartWidget::OnCancel);
	}
}