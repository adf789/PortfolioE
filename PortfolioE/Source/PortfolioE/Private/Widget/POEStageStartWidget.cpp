// Fill out your copyright notice in the Description page of Project Settings.

#include "POEStageStartWidget.h"
#include "Components/Button.h"
#include "Components/WrapBox.h"
#include "POEItemSlotWidget.h"
#include "MyInventoryComponent.h"
#include "POEGameInstance.h"
#include "UIScreenInteraction.h"

void UPOEStageStartWidget::OnStageStart()
{
	UGameplayStatics::OpenLevel(GetWorld(), TEXT("VolatileStage"));
}

void UPOEStageStartWidget::OnCancel()
{
	RemoveFromParent();
	UPOEGameInstance* GameInstance = Cast<UPOEGameInstance>(GetWorld()->GetGameInstance());
	if (GameInstance != nullptr) {
		UUserWidget* InventoryWidget = GameInstance->UIScreenInteraction->GetPanel(EUIPanelName::INVENTORY);
		if (InventoryWidget != nullptr) InventoryWidget->RemoveFromParent();
	}
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