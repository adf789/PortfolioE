// Fill out your copyright notice in the Description page of Project Settings.

#include "POEInventoryAndEquipWidget.h"
#include "Components/WrapBox.h"
#include "POEItemSlotWidget.h"
#include "MyInventoryComponent.h"
#include "Components/Image.h"

void UPOEInventoryAndEquipWidget::InitInventoryView(class UMyInventoryComponent* Inventory) {
	InventoyBox->ClearChildren();
	
	CHECKRETURN(ItemSlotWidgetClass == nullptr);
	UGameInstance* GameInstance = GetWorld()->GetGameInstance();
	for (auto It = Inventory->GetItems().CreateConstIterator(); It; ++It) {
		UPOEItemSlotWidget* InventorySlotWidget = CreateWidget<UPOEItemSlotWidget>(GameInstance, ItemSlotWidgetClass);
		InventoyBox->AddChildWrapBox(InventorySlotWidget);
		InventorySlotWidget->SetItemAndInitView(It->Value);
	}
}

void UPOEInventoryAndEquipWidget::SetEquipItemView(UTexture2D* ItemImage)
{
	EquippedSlot->SetBrushFromTexture(ItemImage);
	EquippedSlot->SetColorAndOpacity(FLinearColor::White);
}

void UPOEInventoryAndEquipWidget::NativeConstruct()
{
	Super::NativeConstruct();

	InventoyBox = Cast<UWrapBox>(GetWidgetFromName(TEXT("InventoryBox")));
	EquippedSlot = Cast<UImage>(GetWidgetFromName(TEXT("EquipSlotItemImage")));
}
