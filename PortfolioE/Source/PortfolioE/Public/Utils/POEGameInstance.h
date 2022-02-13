// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "PortfolioE.h"
#include "Engine/GameInstance.h"
#include "Engine/DataTable.h"
#include "Engine/StreamableManager.h"
#include "UIScreenInteraction.h"
#include "POEGameInstance.generated.h"

USTRUCT(BlueprintType)
struct FPOEItemData : public FTableRowBase {
	GENERATED_BODY()

public:
	FPOEItemData() : ItemId(0), ItemName("Default"), Passive(false), Description(""), DropRate(0) {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	int32 ItemId;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	FString ItemName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	bool Passive;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	FString Description;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	int32 DropRate;
};

USTRUCT(BlueprintType)
struct FPOEItemStatData : public FTableRowBase {
	GENERATED_BODY()

public:
	FPOEItemStatData() : ItemId(0), Level(0), RequireExp(0), AttackValue(0), HpValue(0), SpeedValue(0) {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	int32 ItemId;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	int32 Level;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	int32 RequireExp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	int32 AttackValue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	int32 HpValue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	int32 SpeedValue;
};

USTRUCT(BlueprintType)
struct FPOEItemLotteryData : public FTableRowBase {
	GENERATED_BODY()

public:
	FPOEItemLotteryData() : ItemId(0), Percent(0) {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	int32 ItemId;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	int32 Percent;
};

USTRUCT(BlueprintType)
struct FPOEMonsterStatData : public FTableRowBase {
	GENERATED_BODY()

public:
	FPOEMonsterStatData() : MonsterId(0), MonsterLevel(1), AttackValue(0), HpValue(0), AddAttackValuePerLevel(0), AddHpValuePerLevel(0) {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	int32 MonsterId;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	int32 MonsterLevel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	int32 AttackValue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	int32 HpValue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	int32 AddAttackValuePerLevel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	int32 AddHpValuePerLevel;
};

/**
 * 
 */
UCLASS()
class PORTFOLIOE_API UPOEGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
	
public:
	UPOEGameInstance();
	~UPOEGameInstance();

	FPOEItemData* GetPOEItemData(int32 ItemId);
	FPOEItemStatData* GetPOEItemStatData(int32 ItemId, int32 Level);
	int32 GetLotteryRandomItemId();
	FPOEMonsterStatData* GetMonsterDataForId(int32 MonsterId);

	UFUNCTION()
	class UTexture2D* GetItemTextureForId(int32 ItemId);


	class ActorObjectPool* EffectPooling;
	class ActorObjectPool* DamageTextPooling;
	class ActorObjectPool* MonsterPooling;
	UUIScreenInteraction* UIScreenInteraction;

	int32 LotteryCoinCount;
	int32 LotteryCount;

private:
	UPROPERTY()
	class UDataTable* POEItemDataTable;

	UPROPERTY()
	class UDataTable* POEItemStatTable;

	UPROPERTY()
	class UDataTable* POEItemLotteryTable;

	UPROPERTY()
	class UDataTable* POEMonsterTable;

	UPROPERTY()
	TMap<int32, class UTexture2D*> LoadedTexture;
};
