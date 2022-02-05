// Fill out your copyright notice in the Description page of Project Settings.

#include "InventoryItem_Equipment.h"
#include "POEGameInstance.h"

UInventoryItem_Equipment::UInventoryItem_Equipment() {

}

void UInventoryItem_Equipment::SetItemData(FPOEItemData* ItemData) {
	CHECKRETURN(ItemData == nullptr);

	UPOEGameInstance* GameInstance = Cast<UPOEGameInstance>(GetWorld()->GetGameInstance());
	CHECKRETURN(GameInstance == nullptr);

	FPOEItemStatData* ItemStatData = GameInstance->GetPOEItemStatData(ItemData->ItemId, 1);
	CHECKRETURN(ItemStatData == nullptr);

	SetDisplayName(FName(*ItemData->ItemName));
	SetTextureId(ItemStatData->ItemId - 1);
	SetDescription(FText::FromString(ItemData->Description));
	SetCurrentExp(0);
	SetRequireExp(ItemStatData->RequireExp);
	SetItemType(EItemType::EQUIPMENT);
	this->ItemAttackValue = ItemStatData->AttackValue;
	this->ItemHpValue = ItemStatData->HpValue;
	this->ItemMoveSpeedValue = ItemStatData->SpeedValue;
}
