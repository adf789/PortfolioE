// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "PortfolioE.h"
#include "UObject/NoExportTypes.h"
#include "MyInventoryComponent.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class PORTFOLIOE_API UMyInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UMyInventoryComponent();
	~UMyInventoryComponent();
	
	UFUNCTION()
	bool IsRemainCapacity() const;

	bool IsRemainCapacity(int32 AddCount) const;
	
	bool IsExistItemForItemId(class UInventoryItem_Base* TargetItem) const;

	UFUNCTION()
	bool TryInsertItem(class UInventoryItem_Base* Item);
	
	void DeleteItem(class UInventoryItem_Base* TargetItem);

	UFUNCTION()
	TArray<class UInventoryItem_Base*> GetItems();

	int32 GetItemCount();

	UFUNCTION()
	bool TryEquipActiveItem(class UInventoryItem_Equipment* TryEquipItem);

	UFUNCTION()
	bool TryUnEquipActiveItem();

	UFUNCTION()
	bool TryEquipPassiveItem(class UInventoryItem_Equipment* TryEquipItem);

	UFUNCTION()
	bool TryUnEquipPassiveItem();

	UFUNCTION()
	bool TryEquipItem(class UInventoryItem_Equipment* TryEquipItem);

	UFUNCTION()
	class UInventoryItem_Equipment* GetEquippedActiveItem();

	UFUNCTION()
	class UInventoryItem_Equipment* GetEquippedPassiveItem();

	UFUNCTION()
	void SetDefaultItem();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	virtual void OnRegister() override;


protected:
	UPROPERTY()
	TArray<class UInventoryItem_Base*> HaveItems;

	class TQueue<int> UsableInventoryIds;

	UPROPERTY()
	class UInventoryItem_Equipment* EquippedActiveItem;

	UPROPERTY()
	class UInventoryItem_Equipment* EquippedPassiveItem;

	UPROPERTY()
	class APOECharacter* OwningCharacter;

	UPROPERTY()
	int32 MaxCapacity;
};
