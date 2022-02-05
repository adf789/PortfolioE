// Fill out your copyright notice in the Description page of Project Settings.

#include "InventoryItem_Base.h"

UInventoryItem_Base::UInventoryItem_Base() {

}

void UInventoryItem_Base::SetItemData(FPOEItemData * ItemData)
{
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
