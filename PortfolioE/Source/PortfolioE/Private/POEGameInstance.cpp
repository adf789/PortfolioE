// Fill out your copyright notice in the Description page of Project Settings.

#include "POEGameInstance.h"
#include "ActorObjectPool.h"

UPOEGameInstance::UPOEGameInstance() {
	this->EffectPooling = new ActorObjectPool();
	this->DamageTextPooling = new ActorObjectPool();

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
}

FPOEItemData * UPOEGameInstance::GetPOEItemData(int32 ItemId)
{
	CHECKRETURN(POEItemDataTable == nullptr, nullptr);
	return POEItemDataTable->FindRow<FPOEItemData>(*FString::FromInt(ItemId), TEXT(""));
}

FPOEItemStatData * UPOEGameInstance::GetPOEItemStatData(int32 ItemId, int32 Level)
{
	CHECKRETURN(POEItemStatTable == nullptr, nullptr);
	TArray<FName> ItemDataRowNames = POEItemStatTable->GetRowNames();
	for (int i = 0; i < ItemDataRowNames.Num(); i++) {
		FPOEItemStatData* FetchedItemStatData = POEItemStatTable->FindRow<FPOEItemStatData>(ItemDataRowNames[i], TEXT(""));

		if (FetchedItemStatData->ItemId == ItemId && FetchedItemStatData->Level == Level) return FetchedItemStatData;
	}

	return nullptr;
}
