// Fill out your copyright notice in the Description page of Project Settings.

#include "POELotteryWidget.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "POEGameInstance.h"
#include "POEMessageBoxWidget.h"
#include "POECharacter.h"
#include "MyInventoryComponent.h"
#include "InventoryItem_Equipment.h"

void UPOELotteryWidget::NativeConstruct() {
	Super::NativeConstruct();

	HaveCoinText = Cast<UTextBlock>(GetWidgetFromName(TEXT("Text_HaveCoin")));
	CHECKRETURN(HaveCoinText == nullptr);

	UPOEGameInstance* GameInstance = Cast<UPOEGameInstance>(GetWorld()->GetGameInstance());
	CHECKRETURN(GameInstance == nullptr);
	HaveCoinText->SetText(FText::AsNumber(GameInstance->LotteryCoinCount));

	UButton* StartLotteryButton = Cast<UButton>(GetWidgetFromName(TEXT("Button_StartLottery")));
	CHECKRETURN(StartLotteryButton == nullptr);

	UButton* CloseButton = Cast<UButton>(GetWidgetFromName(TEXT("Button_Close")));
	CHECKRETURN(CloseButton == nullptr);

	StartLotteryButton->OnClicked.AddDynamic(this, &UPOELotteryWidget::StartLottery);
	CloseButton->OnClicked.AddDynamic(this, &UPOELotteryWidget::ClosePanel);
}

int32 UPOELotteryWidget::GetItemIdForRandom()
{
	return int32();
}

void UPOELotteryWidget::StartLottery() {
	UPOEGameInstance* GameInstance = Cast<UPOEGameInstance>(GetWorld()->GetGameInstance());
	CHECKRETURN(GameInstance == nullptr);
	if (GameInstance->LotteryCoinCount <= 0) {
		ShowMessagePanel(TEXT("Not enough coin."));
		return;
	}

	APOECharacter* Character = Cast<APOECharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());
	CHECKRETURN(Character == nullptr);
	if (!Character->Inventory->IsRemainCapacity()) {
		ShowMessagePanel(TEXT("Full Inventory."));
		return;
	}

	GameInstance->LotteryCoinCount--;
	HaveCoinText->SetText(FText::AsNumber(GameInstance->LotteryCoinCount));

	UInventoryItem_Base* NewItem = CreateLotteryItem();
	Character->Inventory->TryInsertItem(NewItem);

	ShowMessagePanel(FString::Printf(TEXT("get %s"), *NewItem->GetDisplayName().ToString()));
}

UInventoryItem_Base* UPOELotteryWidget::CreateLotteryItem() {
	UPOEGameInstance* GameInstance = Cast<UPOEGameInstance>(GetWorld()->GetGameInstance());
	CHECKRETURN(GameInstance == nullptr, nullptr);

	UInventoryItem_Equipment* NewItem = NewObject<UInventoryItem_Equipment>(GameInstance, UInventoryItem_Equipment::StaticClass(), FName(*FString::Printf(TEXT("Lottery%d"), GameInstance->LotteryCount)));
	GameInstance->LotteryCount++;
	NewItem->SetItemData(GameInstance->GetPOEItemData(GameInstance->GetLotteryRandomItemId()));
	AddRandonStatToItem(NewItem);

	return NewItem;
}
void UPOELotteryWidget::ShowMessagePanel(FString Text) {
	UPOEGameInstance* GameInstance = Cast<UPOEGameInstance>(GetWorld()->GetGameInstance());
	CHECKRETURN(GameInstance == nullptr);

	UPOEMessageBoxWidget* MessagePanel = Cast<UPOEMessageBoxWidget>(GameInstance->UIScreenInteraction->ShowPanel(EUIPanelName::MESSAGE_BOX));
	MessagePanel->SetText(Text);
}

void UPOELotteryWidget::AddRandonStatToItem(class UInventoryItem_Equipment* EquipmentItem) {
	if (EquipmentItem->ItemAttackValue != 0) EquipmentItem->ItemAttackValue += FMath::RandRange(0, 50);
	if (EquipmentItem->ItemHpValue != 0) EquipmentItem->ItemHpValue += FMath::RandRange(0, 10000);
	if (EquipmentItem->ItemMoveSpeedValue != 0) EquipmentItem->ItemMoveSpeedValue += FMath::RandRange(0, 20);
}

void UPOELotteryWidget::ClosePanel() {
	UPOEGameInstance* GameInstance = Cast<UPOEGameInstance>(GetWorld()->GetGameInstance());
	CHECKRETURN(GameInstance == nullptr);

	GameInstance->UIScreenInteraction->ClosePanel(EUIPanelName::LOTTERY);
	GetWorld()->GetFirstPlayerController()->SetPause(false);
}

