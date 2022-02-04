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
	virtual void SetItemAndInitView(class UInventoryItem_Base* ItemData);

protected:
	virtual void NativeConstruct() override;

	UFUNCTION()
	virtual void OnUse();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	class UTextBlock* ItemNameText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	class UImage* ItemImage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	class UButton* UseButton;

	class UTexture2D* ItemImage_Texture;

private:
	void OnTextureAssetLoadCompleted();
	
private:
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Item, meta = (AllowPrivateAccess = true))
	class UInventoryItem_Base* ItemData;

	FSoftObjectPath TextureAssetToLoad = FSoftObjectPath(nullptr);
	TSharedPtr<struct FStreamableHandle> AssetStreamingHandle;
};
