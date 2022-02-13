// Fill out your copyright notice in the Description page of Project Settings.

#include "POEStageStartWidget.h"
#include "Components/Button.h"
#include "Components/WrapBox.h"
#include "POEItemSlotWidget.h"
#include "MyInventoryComponent.h"
#include "POEGameInstance.h"
#include "UIScreenInteraction.h"
#include "MonsterSpawner.h"
#include "EngineUtils.h"
#include "POEMonster_Base.h"
#include "Engine/AssetManager.h"
#include "ActorObjectPool.h"

void UPOEStageStartWidget::OnStageStart()
{
	UWorld* CurrentWorld = GetWorld();
	UPOEGameInstance* GameInstance = Cast<UPOEGameInstance>(GetWorld()->GetGameInstance());

	CHECKRETURN(GameInstance == nullptr);

	FSoftObjectPath SlotClassPath(TEXT("/Game/POE/Blueprints/Actor/POEMonster_Normal1.POEMonster_Normal1_C"));

	TSubclassOf<APOEMonster_Base> NormalMonster = UAssetManager::GetStreamableManager().LoadSynchronous(TSoftClassPtr<APOEMonster_Base>(SlotClassPath));
	for (TActorIterator<AMonsterSpawner> It(CurrentWorld); It; ++It) {
		AMonsterSpawner* SpawnPoint = *It;

		for (int i = 0; i < 2; i++) {
			FVector SpawnLocation = SpawnPoint->GetSpawnLocation();

			FRandomStream Random(FMath::Rand());
			int8 RandX = Random.RandRange(-20, 20);
			int8 RandY = Random.RandRange(-20, 20);

			SpawnLocation.X += RandX;
			SpawnLocation.Y += RandY;

			FPOEMonsterStatData* MonsterStatData = GameInstance->GetMonsterDataForId(1);
			CHECKRETURN(MonsterStatData == nullptr);

			APOEMonster_Base* Monster = GameInstance->MonsterPooling->GetUnUseMonster(MonsterStatData->MonsterId);
			if (Monster == nullptr) {
				TEST_LOG("생성!!");
				Monster = GetWorld()->SpawnActor<APOEMonster_Base>(NormalMonster->GetDefaultObject()->GetClass(), SpawnLocation, FRotator::ZeroRotator);
				Monster->MonsterId = MonsterStatData->MonsterId;
				GameInstance->MonsterPooling->AddMonster(Monster);
			}
			else TEST_LOG("찾음!!");

			CHECKRETURN(Monster == nullptr);
			Monster->Active();
			Monster->CharacterStatus->InitAttackValue(MonsterStatData->AttackValue);
			Monster->CharacterStatus->InitHPVale(MonsterStatData->HpValue);
		}
	}
}

void UPOEStageStartWidget::OnCancel()
{
	RemoveFromParent();
	UPOEGameInstance* GameInstance = Cast<UPOEGameInstance>(GetWorld()->GetGameInstance());
	if (GameInstance != nullptr) {
		UUserWidget* InventoryWidget = GameInstance->UIScreenInteraction->GetPanel(EUIPanelName::INVENTORY);
		if (InventoryWidget != nullptr) InventoryWidget->RemoveFromParent();
	}
	GetOwningPlayer()->SetPause(false);
}

void UPOEStageStartWidget::NativeConstruct() {
	Super::NativeConstruct();

	startButton = Cast<UButton>(GetWidgetFromName(TEXT("Btn_Start")));
	if (startButton != nullptr) {
		startButton->OnClicked.AddDynamic(this, &UPOEStageStartWidget::OnStageStart);
	}

	cancelButton = Cast<UButton>(GetWidgetFromName(TEXT("Btn_Cancel")));
	if (cancelButton != nullptr) {
		cancelButton->OnClicked.AddDynamic(this, &UPOEStageStartWidget::OnCancel);
	}
}