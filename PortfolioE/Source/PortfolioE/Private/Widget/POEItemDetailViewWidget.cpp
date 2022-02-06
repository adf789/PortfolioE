// Fill out your copyright notice in the Description page of Project Settings.

#include "POEItemDetailViewWidget.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "POEGameInstance.h"
#include "InventoryItem_Equipment.h"
#include "POEPlayerController.h"
#include "MyInventoryComponent.h"

void UPOEItemDetailViewWidget::SetItemData(UInventoryItem_Base * Item)
{
	ItemData = Item;
	InitView();
}

void UPOEItemDetailViewWidget::InitView()
{
	if (ItemData == nullptr) {
		ItemImage->SetColorAndOpacity(FLinearColor::Transparent);
		AttackValueText->SetText(FText::FromString(TEXT("")));
		HpValueText->SetText(FText::FromString(TEXT("")));
		SpeedValueText->SetText(FText::FromString(TEXT("")));
		ItemTypeText->SetText(FText::FromString(TEXT("")));
	}
	else {
		UPOEGameInstance* GameInstance = Cast<UPOEGameInstance>(GetWorld()->GetGameInstance());
		CHECKRETURN(GameInstance == nullptr);

		ItemImage->SetBrushFromTexture(GameInstance->GetItemTextureForId(ItemData->GetItemId()));
		ItemImage->SetColorAndOpacity(FLinearColor::White);
		if (ItemData->GetItemType() == EItemType::EQUIPMENT) {
			UInventoryItem_Equipment* EquipItemData = Cast<UInventoryItem_Equipment>(ItemData);
			SetAttackValueText((int)EquipItemData->ItemAttackValue);
			SetHpValueText((int)EquipItemData->ItemHpValue);
			SetSpeedValueText((int)EquipItemData->ItemMoveSpeedValue);
			ItemTypeText->SetText(FText::FromString(EquipItemData->IsPassive ? TEXT("(PASSIVE)") : TEXT("(ACTIVE)")));
		}
	}
}

void UPOEItemDetailViewWidget::SetAttackValueText(int32 Value)
{
	bool ValueIsMoreThanEquipped = true;
	UInventoryItem_Equipment* EquipmentItem = Cast<UInventoryItem_Equipment>(ItemData);

	if (EquipmentItem != nullptr) {
		if (!EquipmentItem->IsPassive && ItemData->GetOwningInventory()->GetEquippedActiveItem() != nullptr) {
			ValueIsMoreThanEquipped = Value >= ItemData->GetOwningInventory()->GetEquippedActiveItem()->ItemAttackValue;
		}
		else if (EquipmentItem->IsPassive && ItemData->GetOwningInventory()->GetEquippedPassiveItem() != nullptr) {
			ValueIsMoreThanEquipped = Value >= ItemData->GetOwningInventory()->GetEquippedPassiveItem()->ItemAttackValue;
		}
	}

	if (Value == 0) {
		AttackValueText->SetVisibility(ESlateVisibility::Hidden);
		AttackValueText->RemoveFromParent();
	}
	else {
		AttackValueText->SetVisibility(ESlateVisibility::Visible);
		AttackValueText->SetText(FText::FromString(FString::Printf(TEXT("Attack bonus: %d"), Value)));
		AttackValueText->SetShadowColorAndOpacity(ValueIsMoreThanEquipped ? FLinearColor::Green : FLinearColor::Red);
	}
}

void UPOEItemDetailViewWidget::SetHpValueText(int32 Value)
{
	bool ValueIsMoreThanEquipped = true;
	UInventoryItem_Equipment* EquipmentItem = Cast<UInventoryItem_Equipment>(ItemData);

	if (EquipmentItem != nullptr) {
		if (!EquipmentItem->IsPassive && ItemData->GetOwningInventory()->GetEquippedActiveItem() != nullptr) {
			ValueIsMoreThanEquipped = Value >= ItemData->GetOwningInventory()->GetEquippedActiveItem()->ItemHpValue;
		}
		else if (EquipmentItem->IsPassive && ItemData->GetOwningInventory()->GetEquippedPassiveItem() != nullptr) {
			ValueIsMoreThanEquipped = Value >= ItemData->GetOwningInventory()->GetEquippedPassiveItem()->ItemHpValue;
		}
	}

	if (Value == 0) {
		HpValueText->SetVisibility(ESlateVisibility::Hidden);
		HpValueText->RemoveFromParent();
	}
	else {
		HpValueText->SetVisibility(ESlateVisibility::Visible);
		HpValueText->SetText(FText::FromString(FString::Printf(TEXT("HP bonus: %d"), Value)));
		HpValueText->SetShadowColorAndOpacity(ValueIsMoreThanEquipped ? FLinearColor::Green : FLinearColor::Red);
	}
}

void UPOEItemDetailViewWidget::SetSpeedValueText(int32 Value)
{
	bool ValueIsMoreThanEquipped = true;
	UInventoryItem_Equipment* EquipmentItem = Cast<UInventoryItem_Equipment>(ItemData);

	if (EquipmentItem != nullptr) {
		if (!EquipmentItem->IsPassive && ItemData->GetOwningInventory()->GetEquippedActiveItem() != nullptr) {
			ValueIsMoreThanEquipped = Value >= ItemData->GetOwningInventory()->GetEquippedActiveItem()->ItemMoveSpeedValue;
		}
		else if (EquipmentItem->IsPassive && ItemData->GetOwningInventory()->GetEquippedPassiveItem() != nullptr) {
			ValueIsMoreThanEquipped = Value >= ItemData->GetOwningInventory()->GetEquippedPassiveItem()->ItemMoveSpeedValue;
		}
	}

	if (Value == 0) {
		SpeedValueText->SetVisibility(ESlateVisibility::Hidden);
		SpeedValueText->RemoveFromParent();
	}
	else {
		SpeedValueText->SetVisibility(ESlateVisibility::Visible);
		SpeedValueText->SetText(FText::FromString(FString::Printf(TEXT("Speed bonus: %d"), Value)));
		SpeedValueText->SetShadowColorAndOpacity(ValueIsMoreThanEquipped ? FLinearColor::Green : FLinearColor::Red);
	}
}

void UPOEItemDetailViewWidget::SetLocationAtCursor()
{
	float CursorLocationX, CursorLocationY;
	GetWorld()->GetFirstPlayerController()->GetMousePosition(CursorLocationX, CursorLocationY);
	this->SetPositionInViewport(FVector2D(CursorLocationX, CursorLocationY));
}

void UPOEItemDetailViewWidget::NativeConstruct()
{
	Super::NativeConstruct();
	ItemImage = Cast<UImage>(GetWidgetFromName(TEXT("ItemIconImage")));
	CHECKRETURN(ItemImage == nullptr);

	AttackValueText = Cast<UTextBlock>(GetWidgetFromName(TEXT("AttackText")));
	CHECKRETURN(AttackValueText == nullptr);

	HpValueText = Cast<UTextBlock>(GetWidgetFromName(TEXT("HpText")));
	CHECKRETURN(HpValueText == nullptr);

	SpeedValueText = Cast<UTextBlock>(GetWidgetFromName(TEXT("SpeedText")));
	CHECKRETURN(SpeedValueText == nullptr);

	ItemTypeText = Cast<UTextBlock>(GetWidgetFromName(TEXT("TypeText")));
	CHECKRETURN(ItemTypeText == nullptr);
}

void UPOEItemDetailViewWidget::NativeTick(const FGeometry & MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	SetLocationAtCursor();
}
