// Fill out your copyright notice in the Description page of Project Settings.

#include "POERewardWidget.h"
#include "Components/WrapBox.h"
#include "POERewardItemWidget.h"
#include "POEGameInstance.h"
#include "Engine/AssetManager.h"

void UPOERewardWidget::SetRewardItemList(TArray<class UInventoryItem_Base*> ItemList) 
{
	RewardListView->ClearChildren();

	int32 Size = FMath::Max3(RewardListView->GetChildrenCount(), ItemList.Num(), 9);
	for (int i = 0; i < Size; i++) {
		UPOERewardItemWidget* TempRewardSlot = nullptr;

		if (i >= ItemList.Num()) {
			break;
		}
		else if (i >= RewardListView->GetChildrenCount()) {
			TempRewardSlot = CreateWidget<UPOERewardItemWidget>(GameInstance, RewardListItemClass);
			RewardListView->AddChild(TempRewardSlot);
		}
		else {
			TempRewardSlot = Cast<UPOERewardItemWidget>(RewardListView->GetChildAt(i));
		}

		CHECKRETURN(TempRewardSlot == nullptr);

		TempRewardSlot->SetVisibility(ESlateVisibility::Visible);
		TempRewardSlot->InitItemView(ItemList[i]);
	}
}

void UPOERewardWidget::AddCoinInRewardList(int32 CoinCount)
{
	if (CoinCount == 0) return;

	bool AlreadyAddCoinSlot = false;
	UPOERewardItemWidget* TempRewardSlot = nullptr;
	for (int i = 0; i < RewardListView->GetChildrenCount(); i++) {
		if (i >= 9) {
			return;
		}

		TempRewardSlot = Cast<UPOERewardItemWidget>(RewardListView->GetChildAt(i));
		if (TempRewardSlot->GetVisibility() == ESlateVisibility::Hidden) {
			AlreadyAddCoinSlot = true;
			break;
		}
	}

	if (!AlreadyAddCoinSlot) {
		TempRewardSlot = CreateWidget<UPOERewardItemWidget>(GameInstance, RewardListItemClass);
		RewardListView->AddChild(TempRewardSlot);
	}
	CHECKRETURN(TempRewardSlot == nullptr);

	TempRewardSlot->InitCoinView(CoinCount);
	TempRewardSlot->SetVisibility(ESlateVisibility::Visible);
}

void UPOERewardWidget::NativeConstruct() {
	Super::NativeConstruct();

	RewardListView = Cast<UWrapBox>(GetWidgetFromName(TEXT("Wrap_RewardList")));
	CHECKRETURN(RewardListView == nullptr);

	FSoftObjectPath SlotClassPath(TEXT("/Game/POE/UIWidget/UI_RewardItemWidget.UI_RewardItemWidget_C"));
	TSubclassOf<UUserWidget> RewardSlotClass = UAssetManager::GetStreamableManager().LoadSynchronous(TSoftClassPtr<UUserWidget>(SlotClassPath));
	RewardListItemClass = RewardSlotClass.Get();
	CHECKRETURN(RewardListItemClass == nullptr);

	GameInstance = Cast<UPOEGameInstance>(GetWorld()->GetGameInstance());
	CHECKRETURN(GameInstance == nullptr);
}


