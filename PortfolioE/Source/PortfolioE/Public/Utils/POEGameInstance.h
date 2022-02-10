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

	class ActorObjectPool* EffectPooling;
	class ActorObjectPool* DamageTextPooling;
	UUIScreenInteraction* UIScreenInteraction;
	int32 LotteryCoinCount;

	UFUNCTION()
	class UTexture2D* GetItemTextureForId(int32 ItemId);

private:
	UPROPERTY()
	class UDataTable* POEItemDataTable;

	UPROPERTY()
	class UDataTable* POEItemStatTable;

	UPROPERTY()
	TMap<int32, class UTexture2D*> LoadedTexture;
};
