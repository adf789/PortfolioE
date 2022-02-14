// Fill out your copyright notice in the Description page of Project Settings.

#include "InventoryItem_Equipment.h"
#include "POEGameInstance.h"

UInventoryItem_Equipment::UInventoryItem_Equipment() {

}

void UInventoryItem_Equipment::SetItemData(FPOEItemData* ItemData) {
	Super::SetItemData(ItemData);

	CHECKRETURN(ItemData == nullptr);

	SetDisplayName(FName(*ItemData->ItemName));
	SetItemId(ItemData->ItemId);
	SetTextureId(ItemData->ItemId);
	SetDescription(FText::FromString(ItemData->Description));
	SetItemType(EItemType::EQUIPMENT);
	this->IsPassive = ItemData->Passive;
	SetItemLevel(1);
}

void UInventoryItem_Equipment::SetItemStatData(FPOEItemStatData * StatData)
{
	Super::SetItemStatData(StatData);

	CHECKRETURN(StatData == nullptr);

	SetCurrentExp(0);
	SetRequireExp(StatData->RequireExp);
	SetItemLevel(StatData->Level);
	this->ItemAttackValue = StatData->AttackValue;
	this->ItemHpValue = StatData->HpValue;
	this->ItemMoveSpeedValue = StatData->SpeedValue;
	this->ItemAddAttackValue = 0;
	this->ItemAddHpValue = 0;
	this->ItemAddMoveSpeedValue = 0;
}

bool UInventoryItem_Equipment::AddExp(int32 Exp)
{
	int32 CurrentItemLevel = ItemLevel;
	if (!Super::AddExp(Exp)) return false;

	UPOEGameInstance* GameInstance = Cast<UPOEGameInstance>(GetWorld()->GetGameInstance());
	CHECKRETURN(GameInstance == nullptr, false);

	if (CurrentItemLevel + 1 == ItemLevel) {
		FPOEItemStatData* NextLevelStatData = GameInstance->GetPOEItemStatData(ItemId, ItemLevel);
		if (NextLevelStatData != nullptr) {
			ItemAttackValue = NextLevelStatData->AttackValue;
			ItemHpValue = NextLevelStatData->HpValue;
			ItemMoveSpeedValue = NextLevelStatData->SpeedValue;
		}
	}

	return true;
}

float UInventoryItem_Equipment::GetAttackValue()
{
	return this->ItemAttackValue + this->ItemAddAttackValue;
}

float UInventoryItem_Equipment::GetHpValue()
{
	return this->ItemHpValue + this->ItemAddHpValue;
}

float UInventoryItem_Equipment::GetMoveSpeedValue()
{
	return this->ItemMoveSpeedValue + this->ItemAddMoveSpeedValue;
}
