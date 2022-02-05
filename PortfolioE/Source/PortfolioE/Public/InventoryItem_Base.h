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

	void SetDisplayName(FName Name);
	FName GetDisplayName();

	void SetHaveCount(int32 HaveCount);
	int32 GetHaveCount();

	void SetTextureId(int32 TextureId);
	int32 GetTextureId();

	void SetDescription(FText Description);
	FText GetDescription();

	void SetOwningInventory(class UMyInventoryComponent* OwningInventory);
	class UMyInventoryComponent* GetOwningInventory();

	void SetItemType(EItemType ItemType);
	EItemType GetItemType();

	virtual void Use() PURE_VIRTUAL(UInventoryItem_Base, );

protected:	
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite)
	FName DisplayName;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite)
	int32 HaveCount;
	
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite)
	int32 TextureId;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, meta = (MultiLine = true))
	FText Description;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite)
	class UMyInventoryComponent* OwningInventory;

	EItemType ItemType;
};
