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

void UPOEItemSlotWidget::SetItemAndInitView(UInventoryItem_Base * ItemData)
{
	this->ItemData = ItemData;

	CHECKRETURN(ItemNameText == nullptr);
	ItemNameText->SetText(FText::FromName(ItemData->GetDisplayName()));

	CHECKRETURN(ItemImage == nullptr);
	/*TextureAssetToLoad = GetDefault<UPOEItemTexturePath>()->TexturePaths[ItemData->GetTextureId()];
	AssetStreamingHandle = UAssetManager::GetStreamableManager().RequestAsyncLoad(TextureAssetToLoad, FStreamableDelegate::CreateUObject(this, &UPOEItemSlotWidget::OnTextureAssetLoadCompleted));*/

	UPOEGameInstance* GameInstance = Cast<UPOEGameInstance>(GetWorld()->GetGameInstance());
	CHECKRETURN(GameInstance == nullptr);

	ItemImage_Texture = GameInstance->GetItemTextureForId(ItemData->GetTextureId());
	CHECKRETURN(ItemImage_Texture == nullptr);

	ItemImage->SetBrushFromTexture(ItemImage_Texture);
}

void UPOEItemSlotWidget::NativeConstruct()
{
	Super::NativeConstruct();

	ItemImage = Cast<UImage>(GetWidgetFromName(TEXT("Image_Item")));
	ItemNameText = Cast<UTextBlock>(GetWidgetFromName(TEXT("Text_ItemName")));
	UseButton = Cast<UButton>(GetWidgetFromName(TEXT("EquipButton")));
	CHECKRETURN(UseButton == nullptr);
	UseButton->OnClicked.AddDynamic(this, &UPOEItemSlotWidget::OnUse);
	UseButton->OnHovered.AddDynamic(this, &UPOEItemSlotWidget::OnShowDetailPanel);
	UseButton->OnUnhovered.AddDynamic(this, &UPOEItemSlotWidget::OnHideDetailPanel);
}

void UPOEItemSlotWidget::OnUse()
{
	CHECKRETURN(ItemData == nullptr);
	if (ItemData->GetItemType() == EItemType::EQUIPMENT) {
		UInventoryItem_Equipment* Equipment = Cast<UInventoryItem_Equipment>(ItemData);

		CHECKRETURN(Equipment == nullptr);
		if (ItemData->GetOwningInventory()->TryEquipItem(Equipment)) {
			UPOEGameInstance* GameInstance = Cast<UPOEGameInstance>(GetWorld()->GetGameInstance());
			CHECKRETURN(GameInstance == nullptr);

			UPOEInventoryAndEquipWidget* InventoryAndEquipWidget = Cast<UPOEInventoryAndEquipWidget>(GameInstance->UIScreenInteraction->GetPanel(EUIPanelName::INVENTORY));
			if (InventoryAndEquipWidget != nullptr) {
				OnHideDetailPanel();
				InventoryAndEquipWidget->InitInventoryView(ItemData->GetOwningInventory());
			}
		}
	}
}

void UPOEItemSlotWidget::OnShowDetailPanel()
{
	//GetWorld()->GetTimerManager().SetTimer(DetailPanelLocationHandler, this, &UPOEItemSlotWidget::SetDetailPanelLocationForCursor, .1f, true);

	UPOEGameInstance* GameInstance = Cast<UPOEGameInstance>(GetWorld()->GetGameInstance());
	GameInstance->UIScreenInteraction->ShowPanel(EUIPanelName::DETAIL_VIEW);
	UPOEItemDetailViewWidget* ItemDetailViewWidget = Cast<UPOEItemDetailViewWidget>(GameInstance->UIScreenInteraction->GetPanel(EUIPanelName::DETAIL_VIEW));
	CHECKRETURN(ItemDetailViewWidget == nullptr);

	ItemDetailViewWidget->SetLocationAtCursor();
	ItemDetailViewWidget->SetItemData(ItemData);
}

void UPOEItemSlotWidget::OnHideDetailPanel()
{
	//GetWorld()->GetTimerManager().ClearTimer(DetailPanelLocationHandler);
	UPOEGameInstance* GameInstance = Cast<UPOEGameInstance>(GetWorld()->GetGameInstance());
	GameInstance->UIScreenInteraction->ClosePanel(EUIPanelName::DETAIL_VIEW);
}

void UPOEItemSlotWidget::SetDetailPanelLocationForCursor()
{
	TEST_LOG("SetDetailLocation");
}

void UPOEItemSlotWidget::OnTextureAssetLoadCompleted()
{
	ItemImage_Texture = Cast<UTexture2D>(AssetStreamingHandle->GetLoadedAsset());
	AssetStreamingHandle.Reset();
	CHECKRETURN(ItemImage_Texture == nullptr);

	ItemImage->SetBrushFromTexture(ItemImage_Texture);
}
