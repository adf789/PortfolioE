// Fill out your copyright notice in the Description page of Project Settings.

#include "MyInventoryComponent.h"
#include "InventoryItem_Base.h"


// Sets default values for this component's properties
UMyInventoryComponent::UMyInventoryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	MaxCapacity = 20;
}


UInventoryItem_Base * UMyInventoryComponent::GetItemForName(FName ItemName) const
{
	if (IsExistItem(ItemName)) return HaveItems[ItemName];
	else return nullptr;
}

bool UMyInventoryComponent::IsRemainCapacity() const
{
	if (HaveItems.Num() == MaxCapacity) return false;
	return true;
}

bool UMyInventoryComponent::IsExistItem(FName ItemName) const
{
	return HaveItems.Contains(ItemName);
}

bool UMyInventoryComponent::IsExistItem(UInventoryItem_Base * Item) const
{
	return IsExistItem(Item->GetDisplayName());
}

void UMyInventoryComponent::InsertItem(UInventoryItem_Base * Item)
{
	if (!IsRemainCapacity()) return;

	HaveItems.Add(Item->GetDisplayName(), Item);
	Item->SetOwningInventory(this);
}

void UMyInventoryComponent::DeleteItem(FName ItemName)
{
	if(HaveItems.Contains(ItemName)) HaveItems.Remove(ItemName);
}

void UMyInventoryComponent::DeleteItem(UInventoryItem_Base * Item)
{
	DeleteItem(Item->GetDisplayName());
}

const TMap<FName, class UInventoryItem_Base*> & UMyInventoryComponent::GetItems()
{
	return HaveItems;
}

// Called when the game starts
void UMyInventoryComponent::BeginPlay()
{
	Super::BeginPlay();
}