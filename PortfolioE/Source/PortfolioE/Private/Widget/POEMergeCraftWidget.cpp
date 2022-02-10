// Fill out your copyright notice in the Description page of Project Settings.

#include "POEMergeCraftWidget.h"
#include "Components/Image.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "MyInventoryComponent.h"

void UPOEMergeCraftWidget::SetBaseSlot(class UInventoryItem_Equipment* BaseItem) {

}

void UPOEMergeCraftWidget::UnSetBaseSlot()
{

}

void UPOEMergeCraftWidget::SetMaterialSlot(UInventoryItem_Equipment * BaseItem)
{
}

void UPOEMergeCraftWidget::UnSetMaterialSlot()
{
}

void UPOEMergeCraftWidget::StartMergeCraft()
{
}

void UPOEMergeCraftWidget::NativeConstruct() {
	Super::NativeConstruct();

	UImage* BaseSlotImage = Cast<UImage>(GetWidgetFromName(TEXT("BaseItemImage")));
	CHECKRETURN(BaseSlotImage == nullptr);

	UImage* MaterialSlotImage = Cast<UImage>(GetWidgetFromName(TEXT("MaterialItemImage")));
	CHECKRETURN(MaterialSlotImage == nullptr);

	UProgressBar* ExpBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("PB_ExpValue")));
	CHECKRETURN(ExpBar == nullptr);

	UTextBlock* BaseInfoText = Cast<UTextBlock>(GetWidgetFromName(TEXT("Info_Base")));
	CHECKRETURN(BaseInfoText == nullptr);

	UTextBlock* MaterialInfoText = Cast<UTextBlock>(GetWidgetFromName(TEXT("Info_Material")));
	CHECKRETURN(MaterialInfoText == nullptr);

	UTextBlock* AttackValueText = Cast<UTextBlock>(GetWidgetFromName(TEXT("AttackValue")));
	CHECKRETURN(AttackValueText == nullptr);

	UTextBlock* HpValueText = Cast<UTextBlock>(GetWidgetFromName(TEXT("HpValue")));
	CHECKRETURN(HpValueText == nullptr);

	UTextBlock* SpeedValueText = Cast<UTextBlock>(GetWidgetFromName(TEXT("SpeedValue")));
	CHECKRETURN(SpeedValueText == nullptr);

	UButton* BaseUnSetButton = Cast<UButton>(GetWidgetFromName(TEXT("BaseUnSetButton")));
	CHECKRETURN(BaseUnSetButton == nullptr);

	UButton* MaterialUnSetButton = Cast<UButton>(GetWidgetFromName(TEXT("MaterialUnSetButton")));
	CHECKRETURN(MaterialUnSetButton == nullptr);

	UButton* StartMerghButton = Cast<UButton>(GetWidgetFromName(TEXT("Button_MergeCraft")));
	CHECKRETURN(StartMerghButton == nullptr);

	BaseUnSetButton->OnClicked.AddDynamic(this, &UPOEMergeCraftWidget::UnSetBaseSlot);
	MaterialUnSetButton->OnClicked.AddDynamic(this, &UPOEMergeCraftWidget::UnSetMaterialSlot);
	StartMerghButton->OnClicked.AddDynamic(this, &UPOEMergeCraftWidget::StartMergeCraft);
}