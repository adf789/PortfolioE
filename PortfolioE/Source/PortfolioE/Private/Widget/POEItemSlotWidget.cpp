// Fill out your copyright notice in the Description page of Project Settings.

#include "POEItemSlotWidget.h"
#include "InventoryItem_Equipment.h"
#include "POEItemTexturePath.h"
#include "POEGameInstance.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/Button.h"
#include "engine/AssetManager.h"
#include "POEInventoryAndEquipWidget.h"
#include "UIScreenInteraction.h"
#include "MyInventoryComponent.h"
#include "POEItemDetailViewWidget.h"
#include "POEMergeCraftWidget.h"
#include "POEInventoryAndEquipWidget.h"
#include "POEOnDragItemWidget.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "ItemDragDropOperation.h"

void UPOEItemSlotWidget::SetItemAndInitView(UInventoryItem_Base * ItemData)
{
	this->ItemData = ItemData;

	CHECKRETURN(ItemNameText == nullptr || ItemImage == nullptr || GameInstance == nullptr);
	ItemNameText->SetText(FText::FromName(ItemData->GetDisplayName()));
	UTexture2D* ItemImage_Texture = GameInstance->GetItemTextureForId(ItemData->GetTextureId());
	ItemImage->SetBrushFromTexture(ItemImage_Texture);
}

void UPOEItemSlotWidget::NativeConstruct()
{
	Super::NativeConstruct();

	ItemImage = Cast<UImage>(GetWidgetFromName(TEXT("Image_Item")));
	ItemNameText = Cast<UTextBlock>(GetWidgetFromName(TEXT("Text_ItemName")));
	UseButton = Cast<UButton>(GetWidgetFromName(TEXT("EquipButton")));
	CHECKRETURN(UseButton == nullptr);

	GameInstance = Cast<UPOEGameInstance>(GetWorld()->GetGameInstance());
	CHECKRETURN(GameInstance == nullptr);
}

void UPOEItemSlotWidget::NativeOnMouseEnter(const FGeometry & InGeometry, const FPointerEvent & InMouseEvent)
{
	Super::NativeOnMouseEnter(InGeometry, InMouseEvent);
	OnShowDetailPanel();
}

void UPOEItemSlotWidget::NativeOnMouseLeave(const FPointerEvent & InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);
	OnHideDetailPanel();
}

FReply UPOEItemSlotWidget::NativeOnMouseButtonDown(const FGeometry & InGeometry, const FPointerEvent & InMouseEvent)
{
	FReply Reply = Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
	if (InMouseEvent.GetEffectingButton() == EKeys::LeftMouseButton) {
		Reply.DetectDrag(TakeWidget(), EKeys::LeftMouseButton);
		return Reply;
	}

	return Reply;
}

FReply UPOEItemSlotWidget::NativeOnMouseButtonUp(const FGeometry & InGeometry, const FPointerEvent & InMouseEvent)
{
	return Super::NativeOnMouseButtonUp(InGeometry, InMouseEvent);

	FEventReply Reply;
	Reply.NativeReply = Super::NativeOnMouseButtonUp(InGeometry, InMouseEvent);
	if (InMouseEvent.IsMouseButtonDown(EKeys::LeftMouseButton) && false) {
		OnUse();
		return Reply.NativeReply;
	}

	return Reply.NativeReply;
}

void UPOEItemSlotWidget::OnUse()
{
	CHECKRETURN(ItemData == nullptr);
	if (ItemData->GetItemType() == EItemType::EQUIPMENT) {
		UInventoryItem_Equipment* Equipment = Cast<UInventoryItem_Equipment>(ItemData);

		CHECKRETURN(Equipment == nullptr);

		if (Equipment->GetOwningInventory()->TryEquipItem(Equipment)) {
			UPOEInventoryAndEquipWidget* InventoryAndEquipWidget = Cast<UPOEInventoryAndEquipWidget>(GameInstance->UIScreenInteraction->GetPanel(EUIPanelName::INVENTORY));
			if (InventoryAndEquipWidget != nullptr) {
				OnHideDetailPanel();
				InventoryAndEquipWidget->InitInventoryView();
			}
		}
	}
}

void UPOEItemSlotWidget::OnShowDetailPanel()
{
	GameInstance->UIScreenInteraction->ShowPanel(EUIPanelName::DETAIL_VIEW);
	UPOEItemDetailViewWidget* ItemDetailViewWidget = Cast<UPOEItemDetailViewWidget>(GameInstance->UIScreenInteraction->GetPanel(EUIPanelName::DETAIL_VIEW));
	CHECKRETURN(ItemDetailViewWidget == nullptr);

	ItemDetailViewWidget->SetLocationAtCursor();
	ItemDetailViewWidget->SetItemData(ItemData);
}

void UPOEItemSlotWidget::OnShowMergeCraftPanel()
{
	GameInstance->UIScreenInteraction->ShowPanel(EUIPanelName::MERGE_CRAFT);

	UPOEMergeCraftWidget* MergeCraftWidget = Cast<UPOEMergeCraftWidget>(GameInstance->UIScreenInteraction->GetPanel(EUIPanelName::MERGE_CRAFT));
	CHECKRETURN(MergeCraftWidget == nullptr);
}

void UPOEItemSlotWidget::OnHideDetailPanel()
{
	GameInstance->UIScreenInteraction->ClosePanel(EUIPanelName::DETAIL_VIEW);
}

void UPOEItemSlotWidget::NativeOnDragDetected(const FGeometry & InGeometry, const FPointerEvent & InMouseEvent, UDragDropOperation *& OutOperation)
{
	Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);
	OnHideDetailPanel();
	CHECKRETURN(!DragWidgetClass);

	UPOEOnDragItemWidget* DragItemWidget = CreateWidget<UPOEOnDragItemWidget>(GameInstance, DragWidgetClass);
	CHECKRETURN(DragItemWidget == nullptr);
	DragItemWidget->SetItemData(ItemData);

	UItemDragDropOperation* DragItemOperation = NewObject<UItemDragDropOperation>();
	DragItemOperation->DefaultDragVisual = DragItemWidget;
	DragItemOperation->Pivot = EDragPivot::TopLeft;

	OutOperation = DragItemOperation;
	
	RemoveFromViewport();

}

void UPOEItemSlotWidget::NativeOnDragCancelled(const FDragDropEvent & InDragDropEvent, UDragDropOperation * InOperation)
{
	Super::NativeOnDragCancelled(InDragDropEvent, InOperation);
	TEST_LOG("");

	UPOEInventoryAndEquipWidget* InventoryPanel = Cast<UPOEInventoryAndEquipWidget>(GameInstance->UIScreenInteraction->GetPanel(EUIPanelName::INVENTORY));
	CHECKRETURN(InventoryPanel == nullptr);
	InventoryPanel->InitInventoryView();
}

bool UPOEItemSlotWidget::NativeOnDrop(const FGeometry & InGeometry, const FDragDropEvent & InDragDropEvent, UDragDropOperation * InOperation)
{
	Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);
	OnShowMergeCraftPanel();

	UPOEInventoryAndEquipWidget* InventoryPanel = Cast<UPOEInventoryAndEquipWidget>(GameInstance->UIScreenInteraction->GetPanel(EUIPanelName::INVENTORY));
	CHECKRETURN(InventoryPanel == nullptr, false);
	InventoryPanel->InitInventoryView();
	return true;
}
