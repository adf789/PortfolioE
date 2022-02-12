// Fill out your copyright notice in the Description page of Project Settings.

#include "POEOnDragItemWidget.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "InventoryItem_Base.h"
#include "POEGameInstance.h"

void UPOEOnDragItemWidget::SetItemData(class UInventoryItem_Base* Item) {
	TargetItem = Item;
}

void UPOEOnDragItemWidget::NativeConstruct()
{
	Super::NativeConstruct();

	ItemNameText = Cast<UTextBlock>(GetWidgetFromName(TEXT("Text_ItemName")));
	CHECKRETURN(ItemNameText == nullptr);

	ItemImage = Cast<UImage>(GetWidgetFromName(TEXT("Image_ItemIcon")));
	CHECKRETURN(ItemImage == nullptr);

	CHECKRETURN(TargetItem == nullptr);

	ItemNameText->SetText(FText::FromName(TargetItem->GetDisplayName()));

	UPOEGameInstance* GameInstance = Cast<UPOEGameInstance>(GetWorld()->GetGameInstance());
	CHECKRETURN(GameInstance == nullptr);

	ItemImage->SetBrushFromTexture(GameInstance->GetItemTextureForId(TargetItem->GetItemId()));
}

void UPOEOnDragItemWidget::NativeTick(const FGeometry & MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	//SetLocationAtCursor();
}

void UPOEOnDragItemWidget::SetLocationAtCursor() {
	float CursorLocationX, CursorLocationY;
	GetWorld()->GetFirstPlayerController()->GetMousePosition(CursorLocationX, CursorLocationY);
	this->SetPositionInViewport(FVector2D(CursorLocationX, CursorLocationY));
}
