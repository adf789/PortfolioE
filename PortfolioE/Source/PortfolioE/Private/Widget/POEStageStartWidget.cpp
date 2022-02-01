// Fill out your copyright notice in the Description page of Project Settings.

#include "POEStageStartWidget.h"
#include "Components/Button.h"
#include "Components/WrapBox.h"
#include "POEItemSlotWidget.h"
#include "MyInventoryComponent.h"

void UPOEStageStartWidget::OnStageStart()
{
}

void UPOEStageStartWidget::InitInventoryView(UMyInventoryComponent * Inventory)
{
	inventoyBox->ClearChildren();

	static ConstructorHelpers::FClassFinder<UUserWidget>
		UI_INVENTORY_SLOT_C(TEXT("/Game/POE/UIWidget/UI_InventoryItemWidget.UI_InventoryItemWidget_c"));
	if (UI_INVENTORY_SLOT_C.Succeeded()) {
		ItemSlotWidgetClass = UI_INVENTORY_SLOT_C.Class;
	}

	UGameInstance* GameInstance = GetWorld()->GetGameInstance();
	for (auto It = Inventory->GetItems().CreateConstIterator(); It; ++It) {
		UPOEItemSlotWidget* InventorySlotWidget = CreateWidget<UPOEItemSlotWidget>(GameInstance, ItemSlotWidgetClass);
		InventorySlotWidget->SetItemAndInitView(It->Value);
	}
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

	inventoyBox = Cast<UWrapBox>(GetWidgetFromName(TEXT("InventoryBox")));
}