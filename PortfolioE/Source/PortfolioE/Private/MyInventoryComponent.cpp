// Fill out your copyright notice in the Description page of Project Settings.

#include "MyInventoryComponent.h"
#include "POECharacterStat.h"
#include "POECharacter.h"
#include "POEGameInstance.h"
#include "InventoryItem_Equipment.h"


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

bool UMyInventoryComponent::TrySetEquipmentItem(UInventoryItem_Equipment * TryEquipItem)
{
	CHECKRETURN(TryEquipItem == nullptr || OwningCharacter == nullptr, false);

	if(!TryUnsetEquipmentItem()) return false;

	OwningCharacter->CharacterStatus->AttackValue += TryEquipItem->ItemAttackValue;
	OwningCharacter->CharacterStatus->MaxHPValue += TryEquipItem->ItemHpValue;
	OwningCharacter->CharacterStatus->CurrentHPValue = OwningCharacter->CharacterStatus->MaxHPValue;
	OwningCharacter->CharacterStatus->MoveSpeedValue += TryEquipItem->ItemMoveSpeedValue;
	DeleteItem(TryEquipItem);
	EquippedItem = TryEquipItem;

	OwningCharacter->ApplyCharacterStatus();

	return true;
}

bool UMyInventoryComponent::TryUnsetEquipmentItem()
{
	CHECKRETURN(OwningCharacter == nullptr, false);
	if (EquippedItem == nullptr) return true;

	OwningCharacter->CharacterStatus->AttackValue -= EquippedItem->ItemAttackValue;
	OwningCharacter->CharacterStatus->MaxHPValue -= EquippedItem->ItemHpValue;
	OwningCharacter->CharacterStatus->CurrentHPValue = OwningCharacter->CharacterStatus->MaxHPValue;
	OwningCharacter->CharacterStatus->MoveSpeedValue -= EquippedItem->ItemMoveSpeedValue;
	InsertItem(EquippedItem);
	EquippedItem = nullptr;

	OwningCharacter->ApplyCharacterStatus();

	return true;
}

UInventoryItem_Equipment * UMyInventoryComponent::GetEquippedItem()
{
	return EquippedItem;
}

void UMyInventoryComponent::SetDefaultItem()
{
	UPOEGameInstance* GameInstance = Cast<UPOEGameInstance>(GetWorld()->GetGameInstance());
	CHECKRETURN(GameInstance == nullptr);

	UInventoryItem_Equipment* DefaultItem1 = NewObject<UInventoryItem_Equipment>(this, UInventoryItem_Equipment::StaticClass(), TEXT("TestItem1"));
	DefaultItem1->SetItemData(GameInstance->GetPOEItemData(1));

	UInventoryItem_Equipment* DefaultItem2 = NewObject<UInventoryItem_Equipment>(this, UInventoryItem_Equipment::StaticClass(), TEXT("TestItem2"));
	DefaultItem2->SetItemData(GameInstance->GetPOEItemData(2));

	UInventoryItem_Equipment* DefaultItem3 = NewObject<UInventoryItem_Equipment>(this, UInventoryItem_Equipment::StaticClass(), TEXT("TestItem3"));
	DefaultItem3->SetItemData(GameInstance->GetPOEItemData(3));

	InsertItem(DefaultItem1);
	InsertItem(DefaultItem2);
	InsertItem(DefaultItem3);
}

// Called when the game starts
void UMyInventoryComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UMyInventoryComponent::OnRegister()
{
	Super::OnRegister();
	OwningCharacter = Cast<APOECharacter>(GetOwner());
}
