// Fill out your copyright notice in the Description page of Project Settings.

#include "POEItemSlotWidget.h"
#include "InventoryItem_Base.h"
#include "POEItemTexturePath.h"
#include "POEGameInstance.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/Button.h"

void UPOEItemSlotWidget::SetItemAndInitView(UInventoryItem_Base * ItemData)
{
	this->ItemData = ItemData;

	CHECKRETURN(ItemNameText == nullptr);
	ItemNameText->SetText(FText::FromName(ItemData->GetDisplayName()));

	CHECKRETURN(ItemImage == nullptr);
	TextureAssetToLoad = GetDefault<UPOEItemTexturePath>()->TexturePaths[ItemData->GetTextureId()];
	auto POEGameInstance = Cast<UPOEGameInstance>(GetWorld()->GetGameInstance());
	CHECKRETURN(POEGameInstance == nullptr);
	AssetStreamingHandle = POEGameInstance->StreamableManager.RequestAsyncLoad(TextureAssetToLoad, FStreamableDelegate::CreateUObject(this, &UPOEItemSlotWidget::OnTextureAssetLoadCompleted));
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
}

void UPOEItemSlotWidget::OnTextureAssetLoadCompleted()
{
	UTexture2D* LoadedTexture = Cast<UTexture2D>(AssetStreamingHandle->GetLoadedAsset());
	AssetStreamingHandle.Reset();
	CHECKRETURN(LoadedTexture == nullptr);

	ItemImage->SetBrushFromTexture(LoadedTexture);
}
