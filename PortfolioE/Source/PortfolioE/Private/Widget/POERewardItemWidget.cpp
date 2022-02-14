// Fill out your copyright notice in the Description page of Project Settings.

#include "POERewardItemWidget.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "POEGameInstance.h"
#include "Engine/AssetManager.h"
#include "InventoryItem_Base.h"

void UPOERewardItemWidget::InitItemView(class UInventoryItem_Base* ItemData) {
	CHECKRETURN(ItemData == nullptr);

	UTexture2D* ItemTexture = GameInstance->GetItemTextureForId(ItemData->GetItemId());
	ItemImage->SetBrushFromTexture(ItemTexture);
	LevelText->SetText(FText::FromString(FString::Printf(TEXT("Lv.%d"), ItemData->GetItemLevel())));
	ItemNameText->SetText(FText::FromName(ItemData->GetDisplayName()));
}

void UPOERewardItemWidget::InitCoinView(int32 CoinCount) {
	static UTexture2D* CoinTexture = Cast<UTexture2D>(UAssetManager::GetStreamableManager().LoadSynchronous(FSoftObjectPath(TEXT("/Game/POE/Effects/Textures/ItemImage/T_Coin.T_Coin"))));
	ItemImage->SetBrushFromTexture(CoinTexture);
	LevelText->SetText(FText::FromString(FString::Printf(TEXT("x %d"), CoinCount)));
	ItemNameText->SetText(FText::FromString(TEXT("Coin")));
}

void UPOERewardItemWidget::NativeConstruct() {
	Super::NativeConstruct();

	ItemImage = Cast<UImage>(GetWidgetFromName(TEXT("Image_Item")));
	CHECKRETURN(ItemImage == nullptr);

	LevelText = Cast<UTextBlock>(GetWidgetFromName(TEXT("Text_Level")));
	CHECKRETURN(ItemImage == nullptr);

	ItemNameText = Cast<UTextBlock>(GetWidgetFromName(TEXT("Text_ItemName")));
	CHECKRETURN(ItemImage == nullptr);

	GameInstance = Cast<UPOEGameInstance>(GetWorld()->GetGameInstance());
	CHECKRETURN(GameInstance == nullptr);
}


