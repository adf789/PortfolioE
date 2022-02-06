// Fill out your copyright notice in the Description page of Project Settings.

#include "POEGameInstance.h"
#include "ActorObjectPool.h"
#include "Engine/AssetManager.h"
#include "POEItemTexturePath.h"

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

UPOEGameInstance::~UPOEGameInstance() {
	delete EffectPooling;
	delete DamageTextPooling;
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

UTexture2D * UPOEGameInstance::GetItemTextureForId(int32 ItemId)
{
	if (LoadedTexture.Contains(ItemId)) return LoadedTexture[ItemId];
	else if (GetDefault<UPOEItemTexturePath>()->TexturePaths.Num() <= ItemId) return nullptr;

	UTexture2D* TempTexture = Cast<UTexture2D>(UAssetManager::GetStreamableManager().LoadSynchronous(GetDefault<UPOEItemTexturePath>()->TexturePaths[ItemId]));
	CHECKRETURN(TempTexture == nullptr, nullptr);

	LoadedTexture.Add(ItemId, TempTexture);
	return TempTexture;
}