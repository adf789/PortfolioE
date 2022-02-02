// Fill out your copyright notice in the Description page of Project Settings.

#include "POEInventoryAndEquipWidget.h"
#include "Components/WrapBox.h"
#include "POEItemSlotWidget.h"
#include "MyInventoryComponent.h"

void UPOEInventoryAndEquipWidget::InitInventoryView(class UMyInventoryComponent* Inventory) {
	inventoyBox->ClearChildren();
	
	CHECKRETURN(ItemSlotWidgetClass == nullptr);
	UGameInstance* GameInstance = GetWorld()->GetGameInstance();
	for (auto It = Inventory->GetItems().CreateConstIterator(); It; ++It) {
		UPOEItemSlotWidget* InventorySlotWidget = CreateWidget<UPOEItemSlotWidget>(GameInstance, ItemSlotWidgetClass);
		inventoyBox->AddChildWrapBox(InventorySlotWidget);
		InventorySlotWidget->SetItemAndInitView(It->Value);
	}
}

void UPOEInventoryAndEquipWidget::NativeConstruct()
{
	Super::NativeConstruct();

	inventoyBox = Cast<UWrapBox>(GetWidgetFromName(TEXT("InventoryBox")));
}
