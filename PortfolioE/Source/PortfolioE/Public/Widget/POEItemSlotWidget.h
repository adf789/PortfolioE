// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "PortfolioE.h"
#include "Blueprint/UserWidget.h"
#include "POEItemSlotWidget.generated.h"

/**
 * 
 */
UCLASS()
class PORTFOLIOE_API UPOEItemSlotWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION()
	virtual void SetItemAndInitView(UInventoryItem_Base* ItemData);

protected:
	UFUNCTION()
	virtual void OnUse();

	UFUNCTION()
	virtual void OnShowDetailPanel();

	UFUNCTION()
	virtual void OnShowMergeCraftPanel();
	
	UFUNCTION()
	virtual void OnHideDetailPanel();

	virtual void NativeConstruct() override;
	virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;
	virtual void NativeOnDragCancelled(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;


protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	class UTextBlock* ItemNameText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	class UImage* ItemImage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	class UButton* UseButton;

	class UPOEGameInstance* GameInstance;

	
	
private:
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Item, meta = (AllowPrivateAccess = true))
	class UInventoryItem_Base* ItemData;

	FSoftObjectPath TextureAssetToLoad = FSoftObjectPath(nullptr);
	TSharedPtr<struct FStreamableHandle> AssetStreamingHandle;

	FTimerHandle DetailPanelLocationHandler;
	FEventReply DragReply;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	TSubclassOf<class UPOEOnDragItemWidget> DragWidgetClass;
};
