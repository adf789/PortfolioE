// Fill out your copyright notice in the Description page of Project Settings.

#include "InventoryItem_Base.h"

UInventoryItem_Base::UInventoryItem_Base() {

}

void UInventoryItem_Base::SetDisplayName(FName Name)
{
	this->DisplayName = Name;
}

FName UInventoryItem_Base::GetDisplayName()
{
	return DisplayName;
}

void UInventoryItem_Base::SetHaveCount(int32 HaveCount)
{
	this->HaveCount = HaveCount;
}

int32 UInventoryItem_Base::GetHaveCount()
{
	return HaveCount;
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
