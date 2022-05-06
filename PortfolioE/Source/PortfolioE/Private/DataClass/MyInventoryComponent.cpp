// Description
// 인벤토리의 기능을 담당하는 클래스

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

UMyInventoryComponent::~UMyInventoryComponent() {
	HaveItems.Reset();
}

bool UMyInventoryComponent::IsRemainCapacity() const
{
	if (HaveItems.Num() == MaxCapacity) {

		return false;
	}
	return true;
}

bool UMyInventoryComponent::IsRemainCapacity(int32 AddCount) const
{
	if (HaveItems.Num() + AddCount > MaxCapacity) {

		return false;
	}
	return true;
}

bool UMyInventoryComponent::IsExistItemForItemId(UInventoryItem_Base * TargetItem) const
{
	CHECKRETURN(TargetItem == nullptr, false);
	return HaveItems.ContainsByPredicate([TargetItem](UInventoryItem_Base* Item) -> bool { return Item->GetItemId() == TargetItem->GetItemId(); });
}

bool UMyInventoryComponent::TryInsertItem(UInventoryItem_Base * Item)
{
	CHECKRETURN(Item == nullptr, false);
	if (!IsRemainCapacity()) return false;

	int InventoryId;
	if (UsableInventoryIds.IsEmpty()) InventoryId = HaveItems.Num();
	else UsableInventoryIds.Dequeue(InventoryId);

	Item->SetInventoryId(InventoryId);
	HaveItems.Add(Item);
	Item->SetOwningInventory(this);

	return true;
}

void UMyInventoryComponent::DeleteItem(UInventoryItem_Base * TargetItem)
{
	CHECKRETURN(TargetItem == nullptr);

	if (HaveItems.RemoveSingle(TargetItem) == 1) {
		UsableInventoryIds.Enqueue(TargetItem->GetInventoryId());
		TargetItem->SetInventoryId(0);
	}
}

TArray<class UInventoryItem_Base*> UMyInventoryComponent::GetItems()
{
	return HaveItems;
}

int32 UMyInventoryComponent::GetItemCount() {
	return HaveItems.Num();
}

bool UMyInventoryComponent::TryEquipActiveItem(UInventoryItem_Equipment * TryEquipItem)
{
	CHECKRETURN(TryEquipItem == nullptr || OwningCharacter == nullptr, false);

	if (!TryUnEquipActiveItem()) return false;

	OwningCharacter->CharacterStatus->AttackValue += TryEquipItem->GetAttackValue();
	OwningCharacter->CharacterStatus->MaxHPValue += TryEquipItem->GetHpValue();
	OwningCharacter->CharacterStatus->CurrentHPValue = OwningCharacter->CharacterStatus->MaxHPValue;
	OwningCharacter->CharacterStatus->MoveSpeedValue += TryEquipItem->GetMoveSpeedValue();
	DeleteItem(TryEquipItem);
	EquippedActiveItem = TryEquipItem;

	OwningCharacter->ApplyCharacterStatus();

	return true;
}

bool UMyInventoryComponent::TryUnEquipActiveItem()
{
	CHECKRETURN(OwningCharacter == nullptr, false);
	if (EquippedActiveItem == nullptr) return true;
	else if (!TryInsertItem(EquippedActiveItem)) {
		return false;
	}

	OwningCharacter->CharacterStatus->AttackValue -= EquippedActiveItem->GetAttackValue();
	OwningCharacter->CharacterStatus->MaxHPValue -= EquippedActiveItem->GetHpValue();
	OwningCharacter->CharacterStatus->CurrentHPValue = OwningCharacter->CharacterStatus->MaxHPValue;
	OwningCharacter->CharacterStatus->MoveSpeedValue -= EquippedActiveItem->GetMoveSpeedValue();
	EquippedActiveItem = nullptr;

	OwningCharacter->ApplyCharacterStatus();

	return true;
}

bool UMyInventoryComponent::TryEquipPassiveItem(UInventoryItem_Equipment * TryEquipItem)
{
	CHECKRETURN(TryEquipItem == nullptr || OwningCharacter == nullptr, false);

	if (!TryUnEquipPassiveItem()) return false;

	OwningCharacter->CharacterStatus->AttackValue += TryEquipItem->GetAttackValue();
	OwningCharacter->CharacterStatus->MaxHPValue += TryEquipItem->GetHpValue();
	OwningCharacter->CharacterStatus->CurrentHPValue = OwningCharacter->CharacterStatus->MaxHPValue;
	OwningCharacter->CharacterStatus->MoveSpeedValue += TryEquipItem->GetMoveSpeedValue();
	DeleteItem(TryEquipItem);
	EquippedPassiveItem = TryEquipItem;

	OwningCharacter->ApplyCharacterStatus();

	return true;
}

bool UMyInventoryComponent::TryUnEquipPassiveItem()
{
	CHECKRETURN(OwningCharacter == nullptr, false);
	if (EquippedPassiveItem == nullptr) return true;
	else if (!TryInsertItem(EquippedPassiveItem)) {
		return false;
	}
	
	OwningCharacter->CharacterStatus->AttackValue -= EquippedPassiveItem->GetAttackValue();
	OwningCharacter->CharacterStatus->MaxHPValue -= EquippedPassiveItem->GetHpValue();
	OwningCharacter->CharacterStatus->CurrentHPValue = OwningCharacter->CharacterStatus->MaxHPValue;
	OwningCharacter->CharacterStatus->MoveSpeedValue -= EquippedPassiveItem->GetMoveSpeedValue();
	EquippedPassiveItem = nullptr;

	OwningCharacter->ApplyCharacterStatus();

	return true;
}

bool UMyInventoryComponent::TryEquipItem(UInventoryItem_Equipment * TryEquipItem)
{
	CHECKRETURN(TryEquipItem == nullptr, false);

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

	GameInstance->LotteryCoinCount = 0;

	HaveItems.Reset();

	// 기본 아이템의 형식
	UInventoryItem_Equipment* DefaultItem1 = NewObject<UInventoryItem_Equipment>(GameInstance, UInventoryItem_Equipment::StaticClass(), TEXT("DefaultItem1"));
	DefaultItem1->SetItemData(GameInstance->GetPOEItemData(0));
	DefaultItem1->SetItemStatData(GameInstance->GetPOEItemStatData(0, 1));

	UInventoryItem_Equipment* DefaultItem2 = NewObject<UInventoryItem_Equipment>(GameInstance, UInventoryItem_Equipment::StaticClass(), TEXT("DefaultItem2"));
	DefaultItem2->SetItemData(GameInstance->GetPOEItemData(1));
	DefaultItem2->SetItemStatData(GameInstance->GetPOEItemStatData(1, 10));

	UInventoryItem_Equipment* DefaultItem3 = NewObject<UInventoryItem_Equipment>(GameInstance, UInventoryItem_Equipment::StaticClass(), TEXT("DefaultItem3"));
	DefaultItem3->SetItemData(GameInstance->GetPOEItemData(2));
	DefaultItem3->SetItemStatData(GameInstance->GetPOEItemStatData(2, 1));

	UInventoryItem_Equipment* DefaultItem4 = NewObject<UInventoryItem_Equipment>(GameInstance, UInventoryItem_Equipment::StaticClass(), TEXT("DefaultItem4"));
	DefaultItem4->SetItemData(GameInstance->GetPOEItemData(3));
	DefaultItem4->SetItemStatData(GameInstance->GetPOEItemStatData(3, 1));

	UInventoryItem_Equipment* DefaultItem5 = NewObject<UInventoryItem_Equipment>(GameInstance, UInventoryItem_Equipment::StaticClass(), TEXT("DefaultItem5"));
	DefaultItem5->SetItemData(GameInstance->GetPOEItemData(4));
	DefaultItem5->SetItemStatData(GameInstance->GetPOEItemStatData(4, 10));

	//TryInsertItem(DefaultItem1);
	TryInsertItem(DefaultItem2);
	/*TryInsertItem(DefaultItem3);
	TryInsertItem(DefaultItem4);*/
	TryInsertItem(DefaultItem5);
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