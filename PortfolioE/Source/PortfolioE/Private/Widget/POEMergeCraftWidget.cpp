// Fill out your copyright notice in the Description page of Project Settings.

#include "POEMergeCraftWidget.h"
#include "Components/Image.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "MyInventoryComponent.h"
#include "POEGameInstance.h"
#include "POECharacter.h"
#include "InventoryItem_Equipment.h"
#include "POEInventoryAndEquipWidget.h"
#include "POEMessageBoxWidget.h"

void UPOEMergeCraftWidget::SetBaseSlot(UInventoryItem_Equipment* BaseItem)
{
	CHECKRETURN(BaseItem == nullptr);
	if (BaseItem == this->BaseItem) {
		UnSetBaseSlot();
		return;
	}
	else if (this->BaseItem != nullptr) {
		this->BaseItem->SetSelect(false);
	}
	this->BaseItem = BaseItem;
	BaseItem->SetSelect(true);

	UpdateInventoryUI();

	UTexture2D* ItemTexture = GameInstance->GetItemTextureForId(BaseItem->GetItemId());
	BaseSlotImage->SetBrushFromTexture(ItemTexture);
	BaseSlotImage->SetColorAndOpacity(FLinearColor::White);
	BaseInfoText->SetVisibility(ESlateVisibility::Hidden);
	BaseItemNameText->SetText(FText::FromString(BaseItem->GetDisplayName().ToString()));

	UpdateUIForValues();
}

void UPOEMergeCraftWidget::UnSetBaseSlot()
{
	if (BaseItem == nullptr) return;

	BaseItem->SetSelect(false);
	UpdateInventoryUI();

	BaseItem = nullptr;
	if (BaseItem == nullptr && MaterialItem == nullptr) {
		ClosePanel();
		return;
	}

	BaseSlotImage->SetColorAndOpacity(FLinearColor::Transparent);
	UpdateUIForValues();
}

void UPOEMergeCraftWidget::SetMaterialSlot(UInventoryItem_Equipment * MaterialItem)
{
	CHECKRETURN(MaterialItem == nullptr);
	if (MaterialItem == this->MaterialItem) {
		UnSetMaterialSlot();
		return;
	}
	else if (this->MaterialItem != nullptr) {
		this->MaterialItem->SetSelect(false);
	}

	this->MaterialItem = MaterialItem;
	MaterialItem->SetSelect(true);

	UpdateInventoryUI();

	UTexture2D* ItemTexture = GameInstance->GetItemTextureForId(MaterialItem->GetItemId());
	MaterialSlotImage->SetBrushFromTexture(ItemTexture);
	MaterialSlotImage->SetColorAndOpacity(FLinearColor::White);
	MaterialInfoText->SetVisibility(ESlateVisibility::Hidden);
	MaterialItemNameText->SetText(FText::FromString(MaterialItem->GetDisplayName().ToString()));

	UpdateUIForValues();
}

void UPOEMergeCraftWidget::UnSetMaterialSlot()
{
	if (MaterialItem == nullptr) return;

	MaterialItem->SetSelect(false);
	UpdateInventoryUI();

	MaterialItem = nullptr;
	if (BaseItem == nullptr && MaterialItem == nullptr) {
		ClosePanel();
		return;
	}

	MaterialSlotImage->SetColorAndOpacity(FLinearColor::Transparent);
	UpdateUIForValues();
}

void UPOEMergeCraftWidget::SetItemToSlot(UInventoryItem_Equipment * Item)
{
	if (BaseItem == Item) {
		UnSetBaseSlot();
	}
	else if (MaterialItem == Item) {
		UnSetMaterialSlot();
	}
	else if (BaseItem == nullptr) {
		SetBaseSlot(Item);
	}
	else if (MaterialItem == nullptr) {
		SetMaterialSlot(Item);
	}
}

void UPOEMergeCraftWidget::StartMergeCraft()
{
	if (BaseItem == nullptr || MaterialItem == nullptr) return;
	else if (BaseItem->GetRequireExp() == -1) return;

	BaseItem->AddExp(MaterialItem->GetAddExp());

	APOECharacter* Character = Cast<APOECharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());
	CHECKRETURN(Character == nullptr);

	UMyInventoryComponent* Inventory = Character->Inventory;
	CHECKRETURN(Inventory == nullptr);
	Inventory->DeleteItem(MaterialItem);

	UInventoryItem_Equipment* DeleteMaterialItem = MaterialItem;
	UnSetMaterialSlot();
	DeleteMaterialItem->ConditionalBeginDestroy();

	UPOEMessageBoxWidget* MessagePanel = Cast<UPOEMessageBoxWidget>(GameInstance->UIScreenInteraction->ShowPanel(EUIPanelName::MESSAGE_BOX));
	CHECKRETURN(MessagePanel == nullptr);
	MessagePanel->SetText(TEXT("Success!!"));
}

void UPOEMergeCraftWidget::UpdateUIForValues()
{
	UpdateTextForValues();
	UpdateExpBarForValues();
}

void UPOEMergeCraftWidget::ClosePanel()
{
	GameInstance->UIScreenInteraction->ClosePanel(EUIPanelName::MERGE_CRAFT);
	UPOEInventoryAndEquipWidget* InventoryPanel = Cast<UPOEInventoryAndEquipWidget>(GameInstance->UIScreenInteraction->GetPanel(EUIPanelName::INVENTORY));
	if (InventoryPanel != nullptr) InventoryPanel->SetVisibilityEquipmentPanel(ESlateVisibility::Visible);
}

void UPOEMergeCraftWidget::NativeConstruct() 
{
	Super::NativeConstruct();

	BaseSlotImage = Cast<UImage>(GetWidgetFromName(TEXT("BaseItemImage")));
	CHECKRETURN(BaseSlotImage == nullptr);

	MaterialSlotImage = Cast<UImage>(GetWidgetFromName(TEXT("MaterialItemImage")));
	CHECKRETURN(MaterialSlotImage == nullptr);

	ExpBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("PB_ExpValue")));
	CHECKRETURN(ExpBar == nullptr);

	BaseInfoText = Cast<UTextBlock>(GetWidgetFromName(TEXT("Info_Base")));
	CHECKRETURN(BaseInfoText == nullptr);

	MaterialInfoText = Cast<UTextBlock>(GetWidgetFromName(TEXT("Info_Material")));
	CHECKRETURN(MaterialInfoText == nullptr);

	AttackValueText = Cast<UTextBlock>(GetWidgetFromName(TEXT("AttackValue")));
	CHECKRETURN(AttackValueText == nullptr);

	HpValueText = Cast<UTextBlock>(GetWidgetFromName(TEXT("HpValue")));
	CHECKRETURN(HpValueText == nullptr);

	SpeedValueText = Cast<UTextBlock>(GetWidgetFromName(TEXT("SpeedValue")));
	CHECKRETURN(SpeedValueText == nullptr);

	ExpPercentText = Cast<UTextBlock>(GetWidgetFromName(TEXT("Text_ExpPercent")));
	CHECKRETURN(ExpPercentText == nullptr);

	BaseItemNameText = Cast<UTextBlock>(GetWidgetFromName(TEXT("Text_BaseName")));
	CHECKRETURN(BaseItemNameText == nullptr);

	MaterialItemNameText = Cast<UTextBlock>(GetWidgetFromName(TEXT("Text_MaterialName")));
	CHECKRETURN(MaterialItemNameText == nullptr);

	ItemLevelText = Cast<UTextBlock>(GetWidgetFromName(TEXT("Text_Level")));
	CHECKRETURN(ItemLevelText == nullptr);

	LevelUpInfoText = Cast<UTextBlock>(GetWidgetFromName(TEXT("Info_LevelUp")));
	CHECKRETURN(LevelUpInfoText == nullptr);

	UButton* BaseUnSetButton = Cast<UButton>(GetWidgetFromName(TEXT("BaseUnSetButton")));
	CHECKRETURN(BaseUnSetButton == nullptr);

	UButton* MaterialUnSetButton = Cast<UButton>(GetWidgetFromName(TEXT("MaterialUnSetButton")));
	CHECKRETURN(MaterialUnSetButton == nullptr);

	UButton* StartMergeButton = Cast<UButton>(GetWidgetFromName(TEXT("Button_MergeCraft")));
	CHECKRETURN(StartMergeButton == nullptr);

	GameInstance = Cast<UPOEGameInstance>(GetWorld()->GetGameInstance());
	CHECKRETURN(GameInstance == nullptr);

	BaseUnSetButton->OnClicked.AddDynamic(this, &UPOEMergeCraftWidget::UnSetBaseSlot);
	MaterialUnSetButton->OnClicked.AddDynamic(this, &UPOEMergeCraftWidget::UnSetMaterialSlot);
	StartMergeButton->OnClicked.AddDynamic(this, &UPOEMergeCraftWidget::StartMergeCraft);

}

void UPOEMergeCraftWidget::UpdateTextForValues()
{
	UInventoryItem_Equipment* EquipmentItem = Cast<UInventoryItem_Equipment>(BaseItem);

	if (EquipmentItem != nullptr) {
		FPOEItemStatData* NextLevelStatData = GameInstance->GetPOEItemStatData(EquipmentItem->GetItemId(), EquipmentItem->GetItemLevel() + 1);
		
		if (NextLevelStatData != nullptr && MaterialItem != nullptr && EquipmentItem->GetRequireExp() <= EquipmentItem->GetCurrentExp() + MaterialItem->GetAddExp()) {
			AttackValueText->SetText(FText::FromString(FString::Printf(TEXT("Attack: %d -> %d"), (int)EquipmentItem->GetAttackValue(), (int)(NextLevelStatData->AttackValue + EquipmentItem->ItemAddAttackValue))));
			HpValueText->SetText(FText::FromString(FString::Printf(TEXT("Hp: %d -> %d"), (int)EquipmentItem->GetHpValue(), (int)(NextLevelStatData->HpValue + EquipmentItem->ItemAddHpValue))));
			SpeedValueText->SetText(FText::FromString(FString::Printf(TEXT("Speed: %d -> %d"), (int)EquipmentItem->GetMoveSpeedValue(), (int)(NextLevelStatData->SpeedValue + EquipmentItem->ItemAddMoveSpeedValue))));
			ItemLevelText->SetText(FText::FromString(FString::Printf(TEXT("Level: %d"), EquipmentItem->GetItemLevel() + 1)));
			LevelUpInfoText->SetVisibility(ESlateVisibility::Visible);
			return;
		}
		AttackValueText->SetText(FText::FromString(FString::Printf(TEXT("Attack: %d"), (int)EquipmentItem->GetAttackValue())));
		HpValueText->SetText(FText::FromString(FString::Printf(TEXT("Hp: %d"), (int)EquipmentItem->GetHpValue())));
		SpeedValueText->SetText(FText::FromString(FString::Printf(TEXT("Speed: %d"), (int)EquipmentItem->GetMoveSpeedValue())));
		ItemLevelText->SetText(FText::FromString(FString::Printf(TEXT("Level: %d"), EquipmentItem->GetItemLevel())));
		LevelUpInfoText->SetVisibility(ESlateVisibility::Hidden);
	}
	else {
		AttackValueText->SetText(FText::FromString(TEXT("")));
		HpValueText->SetText(FText::FromString(TEXT("")));
		SpeedValueText->SetText(FText::FromString(TEXT("")));
		ItemLevelText->SetText(FText::FromString(TEXT("")));
		LevelUpInfoText->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UPOEMergeCraftWidget::UpdateExpBarForValues()
{
	if (BaseItem == nullptr) {
		ExpBar->SetPercent(0);
		ExpPercentText->SetText(FText::FromString(TEXT("")));
		return;
	}

	ExpBar->SetFillColorAndOpacity(FLinearColor::Green);
	if (MaterialItem == nullptr) {
		float ExpPercent = (float)BaseItem->GetCurrentExp() / BaseItem->GetRequireExp();
		ExpBar->SetPercent(ExpPercent);
		ExpPercentText->SetText(FText::FromString(FString::Printf(TEXT("%.1f%%"), ExpPercent * 100)));
		return;
	}
	
	int CurrentExp = BaseItem->GetCurrentExp() + MaterialItem->GetAddExp();
	if (CurrentExp >= BaseItem->GetRequireExp())
	{
		FPOEItemStatData* NextLevelStatData = GameInstance->GetPOEItemStatData(BaseItem->GetItemId(), BaseItem->GetItemLevel() + 1);

		if (NextLevelStatData == nullptr) {
			ExpBar->SetPercent(1);
			ExpBar->SetFillColorAndOpacity(FLinearColor::Yellow);
			ExpPercentText->SetText(FText::FromString(TEXT("%100%%")));
			return;
		}

		CurrentExp = CurrentExp - BaseItem->GetRequireExp();
		float ExpPercent = (float)CurrentExp / NextLevelStatData->RequireExp;
		ExpBar->SetPercent(ExpPercent);
		ExpPercentText->SetText(FText::FromString(FString::Printf(TEXT("%.1f%%"), ExpPercent * 100)));
	}
	else {
		float ExpPercent = (float)CurrentExp / BaseItem->GetRequireExp();
		ExpBar->SetPercent(ExpPercent);
		ExpPercentText->SetText(FText::FromString(FString::Printf(TEXT("%.1f%%"), ExpPercent * 100)));
	}
}

void UPOEMergeCraftWidget::UpdateInventoryUI()
{
	UPOEInventoryAndEquipWidget* InventoryPanel = Cast<UPOEInventoryAndEquipWidget>(GameInstance->UIScreenInteraction->GetPanel(EUIPanelName::INVENTORY));
	CHECKRETURN(InventoryPanel == nullptr);
	InventoryPanel->InitInventoryView();
}
