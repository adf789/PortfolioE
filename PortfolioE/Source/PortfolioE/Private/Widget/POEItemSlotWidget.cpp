// Fill out your copyright notice in the Description page of Project Settings.

#include "POEItemSlotWidget.h"
#include "InventoryItem_Base.h"
#include "POEItemTexturePath.h"
#include "POEGameInstance.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/Button.h"
#include "engine/AssetManager.h"
#include "POEInventoryAndEquipWidget.h"
#include "UIScreenInteraction.h"

void UPOEItemSlotWidget::SetItemAndInitView(UInventoryItem_Base * ItemData)
{
	this->ItemData = ItemData;

	CHECKRETURN(ItemNameText == nullptr);
	ItemNameText->SetText(FText::FromName(ItemData->GetDisplayName()));

	CHECKRETURN(ItemImage == nullptr);
	TextureAssetToLoad = GetDefault<UPOEItemTexturePath>()->TexturePaths[ItemData->GetTextureId()];
	AssetStreamingHandle = UAssetManager::GetStreamableManager().RequestAsyncLoad(TextureAssetToLoad, FStreamableDelegate::CreateUObject(this, &UPOEItemSlotWidget::OnTextureAssetLoadCompleted));
}

void UPOEItemSlotWidget::NativeConstruct()
{
	Super::NativeConstruct();

	ItemImage = Cast<UImage>(GetWidgetFromName(TEXT("Image_Item")));
	ItemNameText = Cast<UTextBlock>(GetWidgetFromName(TEXT("Text_ItemName")));
	UseButton = Cast<UButton>(GetWidgetFromName(TEXT("EquipButton")));
	CHECKRETURN(UseButton == nullptr);
	UseButton->OnClicked.AddDynamic(this, &UPOEItemSlotWidget::OnUse);
}

void UPOEItemSlotWidget::OnUse()
{
	CHECKRETURN(ItemData == nullptr);
	ItemData->Use();
	if (ItemData->GetItemType() == EItemType::EQUIPMENT) {
		UPOEGameInstance* GameInstance = Cast<UPOEGameInstance>(GetWorld()->GetGameInstance());
		if (GameInstance != nullptr) {
			UPOEInventoryAndEquipWidget* InventoryAndEquipWidget = Cast<UPOEInventoryAndEquipWidget>(GameInstance->UIScreenInteraction->GetPanel(EUIPanelName::INVENTORY));
			if (InventoryAndEquipWidget != nullptr) InventoryAndEquipWidget->SetEquipItemView(ItemImage_Texture);
		}
	}
}

void UPOEItemSlotWidget::OnTextureAssetLoadCompleted()
{
	ItemImage_Texture = Cast<UTexture2D>(AssetStreamingHandle->GetLoadedAsset());
	AssetStreamingHandle.Reset();
	CHECKRETURN(ItemImage_Texture == nullptr);

	ItemImage->SetBrushFromTexture(ItemImage_Texture);
}
