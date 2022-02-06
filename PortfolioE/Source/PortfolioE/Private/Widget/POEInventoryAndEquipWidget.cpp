// Fill out your copyright notice in the Description page of Project Settings.

#include "POEInventoryAndEquipWidget.h"
#include "Components/WrapBox.h"
#include "POEItemSlotWidget.h"
#include "MyInventoryComponent.h"
#include "Components/Image.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "POECharacter.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "InventoryItem_Equipment.h"
#include "POEItemTexturePath.h"
#include "Engine/AssetManager.h"
#include "POEGameInstance.h"

void UPOEInventoryAndEquipWidget::InitInventoryView(class UMyInventoryComponent* Inventory) {
	InventoyBox->ClearChildren();
	
	CHECKRETURN(ItemSlotWidgetClass == nullptr);
	UGameInstance* GameInstance = GetWorld()->GetGameInstance();
	for (auto It = Inventory->GetItems().CreateConstIterator(); It; ++It) {
		UPOEItemSlotWidget* InventorySlotWidget = CreateWidget<UPOEItemSlotWidget>(GameInstance, ItemSlotWidgetClass);
		InventoyBox->AddChildWrapBox(InventorySlotWidget);
		InventorySlotWidget->SetItemAndInitView(It->Value);
	}

	InitActiveEquipSlot(Inventory->GetEquippedActiveItem());
	InitPassiveEquipSlot(Inventory->GetEquippedPassiveItem());

	APOECharacter* Character = Cast<APOECharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());
	CHECKRETURN(Character == nullptr);

	CharacterValueText_Attack->SetText(FText::FromString(FString::Printf(TEXT("Attack: %d"), (int)Character->CharacterStatus->AttackValue)));
	CharacterValueText_Hp->SetText(FText::FromString(FString::Printf(TEXT("HP: %d"), (int)Character->CharacterStatus->MaxHPValue)));
	CharacterValueText_Speed->SetText(FText::FromString(FString::Printf(TEXT("Speed: %d"), (int)Character->CharacterStatus->MoveSpeedValue)));
}

void UPOEInventoryAndEquipWidget::SetActiveEquipImage(UTexture2D* ItemImage)
{
	if (ItemImage == nullptr) {
		EquippedActiveImage->SetColorAndOpacity(FLinearColor::Transparent);
	}
	else {
		EquippedActiveImage->SetBrushFromTexture(ItemImage);
		EquippedActiveImage->SetColorAndOpacity(FLinearColor::White);
	}
}

void UPOEInventoryAndEquipWidget::SetPassiveEquipImage(UTexture2D * ItemImage)
{
	if (ItemImage == nullptr) {
		EquippedPassiveImage->SetColorAndOpacity(FLinearColor::Transparent);
	}
	else {
		EquippedPassiveImage->SetBrushFromTexture(ItemImage);
		EquippedPassiveImage->SetColorAndOpacity(FLinearColor::White);
	}
}

void UPOEInventoryAndEquipWidget::InitActiveEquipSlot(UInventoryItem_Equipment * EquippedItem)
{
	if (EquippedItem == nullptr) {
		ActiveInfoText->SetVisibility(ESlateVisibility::Visible);
		SetActiveEquipImage(nullptr);
	}
	else {
		ActiveInfoText->SetVisibility(ESlateVisibility::Hidden);

		UPOEGameInstance* GameInstance = Cast<UPOEGameInstance>(GetWorld()->GetGameInstance());
		CHECKRETURN(GameInstance == nullptr);
		UTexture2D* LoadedTexture = GameInstance->GetItemTextureForId(EquippedItem->GetTextureId());
		SetActiveEquipImage(LoadedTexture);
	}
}

void UPOEInventoryAndEquipWidget::OnActiveUnEuquipClick()
{
	APOECharacter* Character = Cast<APOECharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());
	CHECKRETURN(Character == nullptr);

	if (Character->Inventory->TryUnEquipActiveItem()) {
		EquippedActiveImage->SetColorAndOpacity(FLinearColor::Transparent);

		InitInventoryView(Character->Inventory);
	}
}

void UPOEInventoryAndEquipWidget::InitPassiveEquipSlot(UInventoryItem_Equipment * EquippedItem)
{
	if (EquippedItem == nullptr) {
		PassiveInfoText->SetVisibility(ESlateVisibility::Visible);
		SetPassiveEquipImage(nullptr);
	}
	else {
		PassiveInfoText->SetVisibility(ESlateVisibility::Hidden);

		UPOEGameInstance* GameInstance = Cast<UPOEGameInstance>(GetWorld()->GetGameInstance());
		CHECKRETURN(GameInstance == nullptr);
		UTexture2D* LoadedTexture = GameInstance->GetItemTextureForId(EquippedItem->GetTextureId());
		SetPassiveEquipImage(LoadedTexture);
	}
}

void UPOEInventoryAndEquipWidget::OnPassiveUnEuquipClick()
{
	APOECharacter* Character = Cast<APOECharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());
	CHECKRETURN(Character == nullptr);

	if (Character->Inventory->TryUnEquipPassiveItem()) {
		EquippedPassiveImage->SetColorAndOpacity(FLinearColor::Transparent);

		InitInventoryView(Character->Inventory);
	}
}

void UPOEInventoryAndEquipWidget::NativeConstruct()
{
	Super::NativeConstruct();

	FSoftObjectPath SlotClassPath(TEXT("/Game/POE/UIWidget/UI_InventoryItemWidget.UI_InventoryItemWidget_C"));

	TSubclassOf<UUserWidget> UIWidgetClass = UAssetManager::GetStreamableManager().LoadSynchronous(TSoftClassPtr<UUserWidget>(SlotClassPath));
	CHECKRETURN(UIWidgetClass.Get() == nullptr);
	ItemSlotWidgetClass = UIWidgetClass.Get();

	InventoyBox = Cast<UWrapBox>(GetWidgetFromName(TEXT("InventoryBox")));
	CHECKRETURN(InventoyBox == nullptr);

	EquippedActiveImage = Cast<UImage>(GetWidgetFromName(TEXT("ActiveItemImage")));
	CHECKRETURN(EquippedActiveImage == nullptr);

	ActiveInfoText = Cast<UTextBlock>(GetWidgetFromName(TEXT("Info_Active")));
	CHECKRETURN(ActiveInfoText == nullptr);

	EquippedPassiveImage = Cast<UImage>(GetWidgetFromName(TEXT("PassiveItemImage")));
	CHECKRETURN(EquippedPassiveImage == nullptr);

	PassiveInfoText = Cast<UTextBlock>(GetWidgetFromName(TEXT("Info_Passive")));
	CHECKRETURN(PassiveInfoText == nullptr);

	CharacterValueText_Attack = Cast<UTextBlock>(GetWidgetFromName(TEXT("AttackValue")));
	CHECKRETURN(CharacterValueText_Attack == nullptr);

	CharacterValueText_Hp = Cast<UTextBlock>(GetWidgetFromName(TEXT("HpValue")));
	CHECKRETURN(CharacterValueText_Hp == nullptr);

	CharacterValueText_Speed = Cast<UTextBlock>(GetWidgetFromName(TEXT("SpeedValue")));
	CHECKRETURN(CharacterValueText_Speed == nullptr);

	UButton* UnEquipmentActiveItemButton = Cast<UButton>(GetWidgetFromName(TEXT("ActiveEquipmentButton")));
	if (UnEquipmentActiveItemButton != nullptr) {
		UnEquipmentActiveItemButton->OnClicked.AddDynamic(this, &UPOEInventoryAndEquipWidget::OnActiveUnEuquipClick);
	}

	UButton* UnEquipmentPassiveItemButton = Cast<UButton>(GetWidgetFromName(TEXT("PassiveEquipmentButton")));
	if (UnEquipmentPassiveItemButton != nullptr) {
		UnEquipmentPassiveItemButton->OnClicked.AddDynamic(this, &UPOEInventoryAndEquipWidget::OnPassiveUnEuquipClick);
	}
}
