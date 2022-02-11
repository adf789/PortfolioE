// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "PortfolioE.h"
#include "UObject/NoExportTypes.h"
#include "InventoryItem_Base.generated.h"

/**
 * 
 */
UCLASS(Abstract, DefaultToInstanced)
class PORTFOLIOE_API UInventoryItem_Base : public UObject
{
	GENERATED_BODY()

public:
	UInventoryItem_Base();

	virtual void SetItemData(struct FPOEItemData* ItemData);

	void SetInventoryId(int32 ItemId);
	int32 GetInventoryId();

	void SetItemId(int32 ItemId);
	int32 GetItemId();

	void SetDisplayName(FName Name);
	FName GetDisplayName();

	void SetRequireExp(int32 RequireExp);
	int32 GetRequireExp();

	void SetCurrentExp(int32 CurrentExp);
	int32 GetCurrentExp();

	void SetTextureId(int32 TextureId);
	int32 GetTextureId();

	void SetDescription(FText Description);
	FText GetDescription();

	void SetOwningInventory(class UMyInventoryComponent* OwningInventory);
	class UMyInventoryComponent* GetOwningInventory();

	void SetItemType(EItemType ItemType);
	EItemType GetItemType();

protected:	
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite)
	int32 InventoryId;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite)
	int32 ItemId;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite)
	FName DisplayName;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite)
	int32 RequireExp;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite)
	int32 CurrentExp;
	
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite)
	int32 TextureId;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, meta = (MultiLine = true))
	FText Description;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite)
	class UMyInventoryComponent* OwningInventory;

	EItemType ItemType;
};
