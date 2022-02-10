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
	if (HaveItems.Contains(ItemName)) HaveItems.Remove(ItemName);
}

void UMyInventoryComponent::DeleteItem(UInventoryItem_Base * Item)
{
	DeleteItem(Item->GetDisplayName());
}

const TMap<FName, class UInventoryItem_Base*> & UMyInventoryComponent::GetItems()
{
	return HaveItems;
}

bool UMyInventoryComponent::TryEquipActiveItem(UInventoryItem_Equipment * TryEquipItem)
{
	CHECKRETURN(TryEquipItem == nullptr || OwningCharacter == nullptr, false);

	if (!TryUnEquipActiveItem()) return false;

	OwningCharacter->CharacterStatus->AttackValue += TryEquipItem->ItemAttackValue;
	OwningCharacter->CharacterStatus->MaxHPValue += TryEquipItem->ItemHpValue;
	OwningCharacter->CharacterStatus->CurrentHPValue = OwningCharacter->CharacterStatus->MaxHPValue;
	OwningCharacter->CharacterStatus->MoveSpeedValue += TryEquipItem->ItemMoveSpeedValue;
	DeleteItem(TryEquipItem);
	EquippedActiveItem = TryEquipItem;

	OwningCharacter->ApplyCharacterStatus();

	return true;
}

bool UMyInventoryComponent::TryUnEquipActiveItem()
{
	CHECKRETURN(OwningCharacter == nullptr, false);
	if (EquippedActiveItem == nullptr) return true;

	OwningCharacter->CharacterStatus->AttackValue -= EquippedActiveItem->ItemAttackValue;
	OwningCharacter->CharacterStatus->MaxHPValue -= EquippedActiveItem->ItemHpValue;
	OwningCharacter->CharacterStatus->CurrentHPValue = OwningCharacter->CharacterStatus->MaxHPValue;
	OwningCharacter->CharacterStatus->MoveSpeedValue -= EquippedActiveItem->ItemMoveSpeedValue;
	InsertItem(EquippedActiveItem);
	EquippedActiveItem = nullptr;

	OwningCharacter->ApplyCharacterStatus();

	return true;
}

bool UMyInventoryComponent::TryEquipPassiveItem(UInventoryItem_Equipment * TryEquipItem)
{
	CHECKRETURN(TryEquipItem == nullptr || OwningCharacter == nullptr, false);

	if (!TryUnEquipPassiveItem()) return false;

	OwningCharacter->CharacterStatus->AttackValue += TryEquipItem->ItemAttackValue;
	OwningCharacter->CharacterStatus->MaxHPValue += TryEquipItem->ItemHpValue;
	OwningCharacter->CharacterStatus->CurrentHPValue = OwningCharacter->CharacterStatus->MaxHPValue;
	OwningCharacter->CharacterStatus->MoveSpeedValue += TryEquipItem->ItemMoveSpeedValue;
	DeleteItem(TryEquipItem);
	EquippedPassiveItem = TryEquipItem;

	OwningCharacter->ApplyCharacterStatus();

	return true;
}

bool UMyInventoryComponent::TryUnEquipPassiveItem()
{
	CHECKRETURN(OwningCharacter == nullptr, false);
	if (EquippedPassiveItem == nullptr) return true;

	OwningCharacter->CharacterStatus->AttackValue -= EquippedPassiveItem->ItemAttackValue;
	OwningCharacter->CharacterStatus->MaxHPValue -= EquippedPassiveItem->ItemHpValue;
	OwningCharacter->CharacterStatus->CurrentHPValue = OwningCharacter->CharacterStatus->MaxHPValue;
	OwningCharacter->CharacterStatus->MoveSpeedValue -= EquippedPassiveItem->ItemMoveSpeedValue;
	InsertItem(EquippedPassiveItem);
	EquippedPassiveItem = nullptr;

	OwningCharacter->ApplyCharacterStatus();

	return true;
}

bool UMyInventoryComponent::TryEquipItem(UInventoryItem_Equipment * TryEquipItem)
{
	if (TryEquipItem->IsPassive) {
		return TryEquipPassiveItem(TryEquipItem);
	}
	else {
		return TryEquipActiveItem(TryEquipItem);
	}
}

UInventoryItem_Equipment * UMyInventoryComponent::GetEquippedActiveItem()
{
	return EquippedActiveItem;
}

UInventoryItem_Equipment * UMyInventoryComponent::GetEquippedPassiveItem()
{
	return EquippedPassiveItem;
}

void UMyInventoryComponent::SetDefaultItem()
{
	UPOEGameInstance* GameInstance = Cast<UPOEGameInstance>(GetWorld()->GetGameInstance());
	CHECKRETURN(GameInstance == nullptr);

	GameInstance->LotteryCoinCount = 10;

	HaveItems.Reset();

	UInventoryItem_Equipment* DefaultItem1 = NewObject<UInventoryItem_Equipment>(this, UInventoryItem_Equipment::StaticClass(), TEXT("TestItem1"));
	DefaultItem1->SetItemData(GameInstance->GetPOEItemData(0));

	UInventoryItem_Equipment* DefaultItem2 = NewObject<UInventoryItem_Equipment>(this, UInventoryItem_Equipment::StaticClass(), TEXT("TestItem2"));
	DefaultItem2->SetItemData(GameInstance->GetPOEItemData(1));

	UInventoryItem_Equipment* DefaultItem3 = NewObject<UInventoryItem_Equipment>(this, UInventoryItem_Equipment::StaticClass(), TEXT("TestItem3"));
	DefaultItem3->SetItemData(GameInstance->GetPOEItemData(2));

	UInventoryItem_Equipment* DefaultItem4 = NewObject<UInventoryItem_Equipment>(this, UInventoryItem_Equipment::StaticClass(), TEXT("TestItem4"));
	DefaultItem4->SetItemData(GameInstance->GetPOEItemData(3));

	UInventoryItem_Equipment* DefaultItem5 = NewObject<UInventoryItem_Equipment>(this, UInventoryItem_Equipment::StaticClass(), TEXT("TestItem5"));
	DefaultItem5->SetItemData(GameInstance->GetPOEItemData(4));

	InsertItem(DefaultItem1);
	InsertItem(DefaultItem2);
	InsertItem(DefaultItem3);
	InsertItem(DefaultItem4);
	InsertItem(DefaultItem5);
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