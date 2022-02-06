// Fill out your copyright notice in the Description page of Project Settings.

#include "POEInventoryAndEquipWidget.h"
#include "Components/WrapBox.h"
#include "POEItemSlotWidget.h"
#include "MyInventoryComponent.h"
#include "Components/Image.h"
#include "Components/Button.h"
#include "POECharacter.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "InventoryItem_Equipment.h"
#include "POEItemTexturePath.h"
#include "Engine/AssetManager.h"

void UPOEInventoryAndEquipWidget::InitInventoryView(class UMyInventoryComponent* Inventory) {
	InventoyBox->ClearChildren();
	
	CHECKRETURN(ItemSlotWidgetClass == nullptr);
	UGameInstance* GameInstance = GetWorld()->GetGameInstance();
	for (auto It = Inventory->GetItems().CreateConstIterator(); It; ++It) {
		UPOEItemSlotWidget* InventorySlotWidget = CreateWidget<UPOEItemSlotWidget>(GameInstance, ItemSlotWidgetClass);
		InventoyBox->AddChildWrapBox(InventorySlotWidget);
		InventorySlotWidget->SetItemAndInitView(It->Value);
	}

	if (Inventory->GetEquippedItem() != nullptr) {
		FSoftObjectPath TextureAssetToLoad = GetDefault<UPOEItemTexturePath>()->TexturePaths[Inventory->GetEquippedItem()->GetTextureId()];
		UTexture2D* LoadedTexture = Cast<UTexture2D>(UAssetManager::GetStreamableManager().LoadSynchronous(TextureAssetToLoad));
		SetEquipItemView(LoadedTexture);
	}
}

void UPOEInventoryAndEquipWidget::SetEquipItemView(UTexture2D* ItemImage)
{
	EquippedSlot->SetBrushFromTexture(ItemImage);
	EquippedSlot->SetColorAndOpacity(FLinearColor::White);
}

void UPOEInventoryAndEquipWidget::OnUnEuquipButtonClick()
{
	APOECharacter* Character = Cast<APOECharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());
	CHECKRETURN(Character == nullptr);

	if (Character->Inventory->TryUnsetEquipmentItem()) {
		EquippedSlot->SetColorAndOpacity(FLinearColor::Transparent);

		InitInventoryView(Character->Inventory);
	}
}

void UPOEInventoryAndEquipWidget::NativeConstruct()
{
	Super::NativeConstruct();

	InventoyBox = Cast<UWrapBox>(GetWidgetFromName(TEXT("InventoryBox")));
	EquippedSlot = Cast<UImage>(GetWidgetFromName(TEXT("EquipSlotItemImage")));
	UButton* UnEquipmentButton = Cast<UButton>(GetWidgetFromName(TEXT("UnEquipmentButton")));
	if (UnEquipmentButton != nullptr) {
		UnEquipmentButton->OnClicked.AddDynamic(this, &UPOEInventoryAndEquipWidget::OnUnEuquipButtonClick);
	}
}
