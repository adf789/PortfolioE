// Fill out your copyright notice in the Description page of Project Settings.

#include "InventoryItem_Base.h"
#include "POEGameInstance.h"

UInventoryItem_Base::UInventoryItem_Base() {

}

void UInventoryItem_Base::SetItemData(FPOEItemData * ItemData)
{
}

bool UInventoryItem_Base::AddExp(int32 Exp)
{
	if (RequireExp == -1) return false;

	CurrentExp += Exp;
	if (CurrentExp >= RequireExp) {
		CurrentExp -= RequireExp;

		UPOEGameInstance* GameInstance = Cast<UPOEGameInstance>(GetWorld()->GetGameInstance());
		CHECKRETURN(GameInstance == nullptr, false);

		FPOEItemStatData* NextLevelStatData = GameInstance->GetPOEItemStatData(ItemId, ItemLevel + 1);
		if (NextLevelStatData != nullptr) {
			ItemLevel += 1;
			RequireExp = NextLevelStatData->RequireExp;
		}
	}

	return true;
}

void UInventoryItem_Base::SetInventoryId(int32 InventoryId)
{
	this->InventoryId = InventoryId;
}

int32 UInventoryItem_Base::GetInventoryId()
{
	return InventoryId;
}

void UInventoryItem_Base::SetItemId(int32 ItemId)
{
	this->ItemId = ItemId;
}

int32 UInventoryItem_Base::GetItemId()
{
	return ItemId;
}

void UInventoryItem_Base::SetDisplayName(FName Name)
{
	this->DisplayName = Name;
}

FName UInventoryItem_Base::GetDisplayName()
{
	return DisplayName;
}

void UInventoryItem_Base::SetRequireExp(int32 RequireExp)
{
	this->RequireExp = RequireExp;
}

int32 UInventoryItem_Base::GetRequireExp()
{
	return RequireExp;
}

void UInventoryItem_Base::SetCurrentExp(int32 CurrentExp)
{
	this->CurrentExp = CurrentExp;
}

int32 UInventoryItem_Base::GetCurrentExp()
{
	return CurrentExp;
}

void UInventoryItem_Base::SetTextureId(int32 TextureId)
{
	this->TextureId = TextureId;
}

int32 UInventoryItem_Base::GetTextureId()
{
	return TextureId;
}

void UInventoryItem_Base::SetDescription(FText Description)
{
	this->Description = Description;
}

FText UInventoryItem_Base::GetDescription()
{
	return Description;
}

void UInventoryItem_Base::SetItemLevel(int32 Level)
{
	ItemLevel = Level;
}

int32 UInventoryItem_Base::GetItemLevel()
{
	return ItemLevel;
}

int32 UInventoryItem_Base::GetAddExp()
{
	return ItemLevel * 30;
}

void UInventoryItem_Base::SetSelect(bool Select)
{
	IsSelected = Select;
}

bool UInventoryItem_Base::IsSelect()
{
	return IsSelected;
}

void UInventoryItem_Base::SetOwningInventory(UMyInventoryComponent * OwningInventory)
{
	this->OwningInventory = OwningInventory;
}

UMyInventoryComponent * UInventoryItem_Base::GetOwningInventory()
{
	return OwningInventory;
}

void UInventoryItem_Base::SetItemType(EItemType ItemType)
{
	this->ItemType = ItemType;
}

EItemType UInventoryItem_Base::GetItemType()
{
	return ItemType;
}
