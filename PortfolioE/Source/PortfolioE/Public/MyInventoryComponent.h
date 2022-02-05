// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "PortfolioE.h"
#include "Components/ActorComponent.h"
#include "MyInventoryComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PORTFOLIOE_API UMyInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UMyInventoryComponent();

	UFUNCTION()
	class UInventoryItem_Base* GetItemForName(FName ItemName) const;

	UFUNCTION()
	bool IsRemainCapacity() const;

	UFUNCTION()
	bool IsExistItem(FName ItemName) const;

	bool IsExistItem(class UInventoryItem_Base* Item) const;

	UFUNCTION()
	void InsertItem(class UInventoryItem_Base* Item);

	UFUNCTION()
	void DeleteItem(FName ItemName);

	void DeleteItem(class UInventoryItem_Base* Item);

	UFUNCTION()
	const TMap<FName, class UInventoryItem_Base*>& GetItems();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;


protected:
	UPROPERTY()
	TMap<FName, class UInventoryItem_Base*> HaveItems;

	UPROPERTY()
	int32 MaxCapacity;
};
