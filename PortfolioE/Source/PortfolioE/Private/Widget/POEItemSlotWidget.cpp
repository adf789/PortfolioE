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
#include "Engine/AssetManager.h"

void UPOEItemSlotWidget::SetItemAndInitView(UInventoryItem_Base * ItemData)
{
	this->ItemData = ItemData;

	ItemNameText->SetText(FText::FromName(ItemData->GetDisplayName()));
	UTexture2D* ItemImage_Texture = GameInstance->GetItemTextureForId(ItemData->GetTextureId());
	ItemImage->SetBrushFromTexture(ItemImage_Texture);
	SelectedDimImage->SetVisibility(ItemData->IsSelect() ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
}

void UPOEItemSlotWidget::NativeConstruct()
{
	Super::NativeConstruct();

	ItemImage = Cast<UImage>(GetWidgetFromName(TEXT("Image_Item")));
	CHECKRETURN(ItemImage == nullptr);

	ItemNameText = Cast<UTextBlock>(GetWidgetFromName(TEXT("Text_ItemName")));
	CHECKRETURN(ItemNameText == nullptr);

	SelectedDimImage = GetWidgetFromName(TEXT("Image_SelectedDim"));
	CHECKRETURN(SelectedDimImage == nullptr);

	GameInstance = Cast<UPOEGameInstance>(GetWorld()->GetGameInstance());
	CHECKRETURN(GameInstance == nullptr);

	FSoftObjectPath SlotClassPath(TEXT("/Game/POE/UIWidget/UI_OnDragItemWidget.UI_OnDragItemWidget_c"));

	DragWidgetClass = UAssetManager::GetStreamableManager().LoadSynchronous(TSoftClassPtr<UPOEOnDragItemWidget>(SlotClassPath));
	CHECKRETURN(DragWidgetClass == nullptr);
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
	}

	return Reply;
}

void UPOEItemSlotWidget::OnUse()
{
	CHECKRETURN(ItemData == nullptr);
	if (ItemData->GetItemType() == EItemType::EQUIPMENT) {
		UInventoryItem_Equipment* Equipment = Cast<UInventoryItem_Equipment>(ItemData);

		UPOEMergeCraftWidget* MergeCraftPanel = Cast<UPOEMergeCraftWidget>(GameInstance->UIScreenInteraction->GetPanel(EUIPanelName::MERGE_CRAFT));

		if (MergeCraftPanel != nullptr) {
			MergeCraftPanel->SetItemToSlot(Equipment);
		}
		else if (Equipment->GetOwningInventory()->TryEquipItem(Equipment)) {
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

void UPOEItemSlotWidget::OnShowMergeCraftPanel(UInventoryItem_Base* MaterialItemData)
{
	GameInstance->UIScreenInteraction->ShowPanel(EUIPanelName::MERGE_CRAFT);

	UPOEMergeCraftWidget* MergeCraftPanel = Cast<UPOEMergeCraftWidget>(GameInstance->UIScreenInteraction->GetPanel(EUIPanelName::MERGE_CRAFT));
	CHECKRETURN(MergeCraftPanel == nullptr);

	UInventoryItem_Equipment* BaseItem = Cast<UInventoryItem_Equipment>(ItemData);
	UInventoryItem_Equipment* MaterialItem = Cast<UInventoryItem_Equipment>(MaterialItemData);
	CHECKRETURN(BaseItem == nullptr);
	MergeCraftPanel->SetItemToSlot(BaseItem);
	MergeCraftPanel->SetItemToSlot(MaterialItem);
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
	DragItemOperation->SelectedItem = ItemData;

	OutOperation = DragItemOperation;
	
	RemoveFromViewport();

}

void UPOEItemSlotWidget::NativeOnDragCancelled(const FDragDropEvent & InDragDropEvent, UDragDropOperation * InOperation)
{
	Super::NativeOnDragCancelled(InDragDropEvent, InOperation);

	UPOEInventoryAndEquipWidget* InventoryPanel = Cast<UPOEInventoryAndEquipWidget>(GameInstance->UIScreenInteraction->GetPanel(EUIPanelName::INVENTORY));
	CHECKRETURN(InventoryPanel == nullptr);
	InventoryPanel->InitInventoryView();
}

bool UPOEItemSlotWidget::NativeOnDrop(const FGeometry & InGeometry, const FDragDropEvent & InDragDropEvent, UDragDropOperation * InOperation)
{
	Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);

	UItemDragDropOperation* DragItemOperation = Cast<UItemDragDropOperation>(InOperation);
	OnShowMergeCraftPanel(DragItemOperation->SelectedItem);

	UPOEInventoryAndEquipWidget* InventoryPanel = Cast<UPOEInventoryAndEquipWidget>(GameInstance->UIScreenInteraction->GetPanel(EUIPanelName::INVENTORY));
	CHECKRETURN(InventoryPanel == nullptr, false);
	InventoryPanel->InitInventoryView();
	InventoryPanel->SetVisibilityEquipmentPanel(ESlateVisibility::Hidden);
	return true;
}

FReply UPOEItemSlotWidget::NativeOnMouseButtonDoubleClick(const FGeometry & InGeometry, const FPointerEvent & InMouseEvent)
{
	if (InMouseEvent.GetEffectingButton() == EKeys::LeftMouseButton) {
		OnUse();
	}
	return Super::NativeOnMouseButtonDoubleClick(InGeometry, InMouseEvent);
}
