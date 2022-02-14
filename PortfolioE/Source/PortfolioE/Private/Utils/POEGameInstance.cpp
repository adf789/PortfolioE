// Fill out your copyright notice in the Description page of Project Settings.

#include "POEGameInstance.h"
#include "ActorObjectPool.h"
#include "Engine/AssetManager.h"
#include "POEItemTexturePath.h"
#include "POERewardWidget.h"
#include "POECharacter.h"
#include "MyInventoryComponent.h"
#include "InventoryItem_Equipment.h"

UPOEGameInstance::UPOEGameInstance() {
	this->EffectPooling = new ActorObjectPool();
	this->DamageTextPooling = new ActorObjectPool();
	this->MonsterPooling = new ActorObjectPool();

	static ConstructorHelpers::FObjectFinder<UDataTable>
		DT_ITEM_DATA_TABLE(TEXT("/Game/POE/GameData/ItemDataTable.ItemDataTable"));
	CHECKRETURN(!DT_ITEM_DATA_TABLE.Succeeded());
	POEItemDataTable = DT_ITEM_DATA_TABLE.Object;
	CHECKRETURN(POEItemDataTable->RowMap.Num() == 0);

	static ConstructorHelpers::FObjectFinder<UDataTable>
		DT_ITEM_STAT_TABLE(TEXT("/Game/POE/GameData/ItemStatTable.ItemStatTable"));
	CHECKRETURN(!DT_ITEM_STAT_TABLE.Succeeded());
	POEItemStatTable = DT_ITEM_STAT_TABLE.Object;
	CHECKRETURN(POEItemStatTable->RowMap.Num() == 0);

	static ConstructorHelpers::FObjectFinder<UDataTable>
		DT_ITEM_LOTTERY_TABLE(TEXT("/Game/POE/GameData/ItemLotteryTable.ItemLotteryTable"));
	CHECKRETURN(!DT_ITEM_LOTTERY_TABLE.Succeeded());
	POEItemLotteryTable = DT_ITEM_LOTTERY_TABLE.Object;
	CHECKRETURN(POEItemLotteryTable->RowMap.Num() == 0);

	static ConstructorHelpers::FObjectFinder<UDataTable>
		DT_MONSTER_TABLE(TEXT("/Game/POE/GameData/MonsterTable.MonsterTable"));
	CHECKRETURN(!DT_MONSTER_TABLE.Succeeded());
	POEMonsterTable = DT_MONSTER_TABLE.Object;
	CHECKRETURN(POEMonsterTable->RowMap.Num() == 0);

	MaxStageLevel = 1;
	CurStageLevel = 1;
}

UPOEGameInstance::~UPOEGameInstance() {
	delete EffectPooling;
	delete DamageTextPooling;
	delete MonsterPooling;

	CreatedRewardItems.Reset();
	LoadedTexture.Reset();
}

FPOEItemData * UPOEGameInstance::GetPOEItemData(int32 ItemId)
{
	CHECKRETURN(POEItemDataTable == nullptr, nullptr);
	return POEItemDataTable->FindRow<FPOEItemData>(*FString::FromInt(ItemId + 1), TEXT(""));
}

FPOEItemStatData * UPOEGameInstance::GetPOEItemStatData(int32 ItemId, int32 Level)
{
	CHECKRETURN(POEItemStatTable == nullptr, nullptr);
	TArray<FName> ItemDataRowNames = POEItemStatTable->GetRowNames();
	for (int i = 0; i < ItemDataRowNames.Num(); i++) {
		FPOEItemStatData* FetchedItemStatData = POEItemStatTable->FindRow<FPOEItemStatData>(ItemDataRowNames[i], TEXT(""));

		if (FetchedItemStatData->ItemId == ItemId && FetchedItemStatData->Level == Level) return FetchedItemStatData;
	}

	TEST_LOG_WITH_VAR("Not found stat data id: %d, level: %d", ItemId, Level);
	return nullptr;
}

FPOEMonsterStatData * UPOEGameInstance::GetMonsterDataForId(int32 MonsterId)
{
	CHECKRETURN(POEMonsterTable == nullptr, nullptr);
	return POEMonsterTable->FindRow<FPOEMonsterStatData>(*FString::FromInt(MonsterId + 1), TEXT(""));
}

void UPOEGameInstance::SetCountSpawnMonster(int32 MonsterCount)
{
	MaxSpawnMonsterCount = MonsterCount;
	CurSpawnMonsterCount = MaxSpawnMonsterCount;
}

void UPOEGameInstance::DyingMonster()
{
	if (CurSpawnMonsterCount <= 0) return;
	CurSpawnMonsterCount--;

	if (CurSpawnMonsterCount <= 0) {
		if (MAX_STAGE > MaxStageLevel && MaxStageLevel == CurStageLevel) {
			MaxStageLevel++;
			CurStageLevel = MaxStageLevel;
		}
		CreateRewardItems();
		RewardItemInInventory();
		ShowBattleReward();
	}
}

void UPOEGameInstance::ShowBattleReward()
{
	UPOERewardWidget* RewardPanel = Cast<UPOERewardWidget>(UIScreenInteraction->ShowPanel(EUIPanelName::REWARD));
	CHECKRETURN(RewardPanel == nullptr);

	RewardPanel->SetRewardItemList(CreatedRewardItems);
	RewardPanel->AddCoinInRewardList(RewardCoinCount);

	FTimerHandle VisibleTimer;
	GetWorld()->GetTimerManager().SetTimer(VisibleTimer, [this]() {
		UIScreenInteraction->ClosePanel(EUIPanelName::REWARD);
		IsDoingBattle = false;

		}, 1.0f, false, 5.0f);
}

void UPOEGameInstance::CreateRewardItems()
{
	APOECharacter* Character = Cast<APOECharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());
	CHECKRETURN(Character == nullptr);

	CreatedRewardItems.Reset();
	RewardCoinCount = 0;
	FRandomStream Random(FMath::Rand());
	Random.GenerateNewSeed();

	int32 AddItemCount = 0;
	for (int i = 0, RewardCoinCount = 0; i < MaxSpawnMonsterCount; i++) {
		RewardCoinCount += Random.RandRange(0, CurStageLevel);
		Random.GenerateNewSeed();

		if (Character->Inventory->IsRemainCapacity(AddItemCount)) {
			UInventoryItem_Equipment* EquipmentItem = GetRandEquipmentForReward(CurStageLevel);
			CreatedRewardItems.Add(EquipmentItem);
			AddItemCount++;
		}
	}
}

void UPOEGameInstance::RewardItemInInventory()
{
	APOECharacter* Character = Cast<APOECharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());
	CHECKRETURN(Character == nullptr);

	LotteryCoinCount += RewardCoinCount;

	for (int i = 0; i < CreatedRewardItems.Num(); i++) {
		if (!Character->Inventory->TryInsertItem(CreatedRewardItems[i])) break;
	}
}

int32 UPOEGameInstance::GetLotteryRandomItemId()
{
	FRandomStream Random(FMath::Rand());
	int8 RandNum = Random.RandRange(0, 100);
	int8 Sum = 0;

	TArray<FName> LotteryDataRowNames = POEItemLotteryTable->GetRowNames();
	for (int i = 0; i < LotteryDataRowNames.Num(); i++) {
		FPOEItemLotteryData* FetchedLotteryData = POEItemLotteryTable->FindRow<FPOEItemLotteryData>(LotteryDataRowNames[i], TEXT(""));
		CHECKRETURN(FetchedLotteryData == nullptr, 0);

		Sum += FetchedLotteryData->Percent;
		if (RandNum <= Sum) {
			return FetchedLotteryData->ItemId;
		}
	}
	return 0;
}

UTexture2D * UPOEGameInstance::GetItemTextureForId(int32 ItemId)
{
	if (LoadedTexture.Contains(ItemId)) return LoadedTexture[ItemId];
	else if (GetDefault<UPOEItemTexturePath>()->TexturePaths.Num() <= ItemId) return nullptr;

	UTexture2D* TempTexture = Cast<UTexture2D>(UAssetManager::GetStreamableManager().LoadSynchronous(GetDefault<UPOEItemTexturePath>()->TexturePaths[ItemId]));
	CHECKRETURN(TempTexture == nullptr, nullptr);

	LoadedTexture.Add(ItemId, TempTexture);
	return TempTexture;
}

UInventoryItem_Equipment * UPOEGameInstance::GetRandEquipmentForReward(int32 Level)
{
	// ����� ����
	SetMaxDropPercent();

	// ������ ���
	FPOEItemData* DropItemData = GetDropItemData();

	// ������ ����
	return GetCreatedDropItem(DropItemData, Level);
}

void UPOEGameInstance::SetMaxDropPercent() {
	if (MaxDropPercent != 0) return;

	MaxDropPercent = 30;	// �ƹ��ŵ� ������ ���� Ȯ��

	TArray<FName> DropItemDataRowNames = POEItemDataTable->GetRowNames();
	for (int i = 0; i < DropItemDataRowNames.Num(); i++) {
		FPOEItemData* FetchedItemData = POEItemDataTable->FindRow<FPOEItemData>(DropItemDataRowNames[i], TEXT(""));
		if (FetchedItemData == nullptr) continue;

		MaxDropPercent += FetchedItemData->DropRate;
	}
}

FPOEItemData * UPOEGameInstance::GetDropItemData()
{
	FRandomStream Random(FMath::Rand());
	int32 RandNum = Random.RandRange(0, MaxDropPercent);
	int32 Sum = 0;

	FPOEItemData* FetchedItemData = nullptr;
	TArray<FName> DropItemDataRowNames = POEItemDataTable->GetRowNames();
	for (int i = 0; i < DropItemDataRowNames.Num(); i++) {
		FetchedItemData = POEItemDataTable->FindRow<FPOEItemData>(DropItemDataRowNames[i], TEXT(""));
		if (FetchedItemData == nullptr) continue;

		Sum += FetchedItemData->DropRate;
		if (RandNum <= Sum) {
			break;
		}
	}

	return FetchedItemData;
}

UInventoryItem_Equipment* UPOEGameInstance::GetCreatedDropItem(FPOEItemData * ItemData, int32 Level)
{
	if (ItemData == nullptr) return nullptr;

	UInventoryItem_Equipment* CreatedItem = NewObject<UInventoryItem_Equipment>(this, UInventoryItem_Equipment::StaticClass(), FName(*FString::Printf(TEXT("Drop%d"), DropCount)));
	CreatedItem->SetItemData(ItemData);
	CreatedItem->SetItemStatData(GetPOEItemStatData(ItemData->ItemId, Level));

	return CreatedItem;
}