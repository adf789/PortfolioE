// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "PortfolioE.h"
#include "InventoryItem_Base.h"
#include "InventoryItem_Equipment.generated.h"

/**
 * 
 */
UCLASS()
class PORTFOLIOE_API UInventoryItem_Equipment : public UInventoryItem_Base
{
	GENERATED_BODY()
	
public:
	UInventoryItem_Equipment();
	
	virtual void SetItemData(struct FPOEItemData* ItemData) override;
	virtual void SetItemStatData(struct FPOEItemStatData* StatData) override;
	virtual bool AddExp(int32 Exp) override;

	float GetAttackValue();
	float GetHpValue();
	float GetMoveSpeedValue();

	float ItemAttackValue;
	float ItemAddAttackValue;

	float ItemHpValue;
	float ItemAddHpValue;

	float ItemMoveSpeedValue;
	float ItemAddMoveSpeedValue;
	bool IsPassive;
};
