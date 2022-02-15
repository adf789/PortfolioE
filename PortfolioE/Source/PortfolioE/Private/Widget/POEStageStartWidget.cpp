// Fill out your copyright notice in the Description page of Project Settings.

#include "POEStageStartWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "POEItemSlotWidget.h"
#include "MyInventoryComponent.h"
#include "POEGameInstance.h"
#include "UIScreenInteraction.h"
#include "MonsterSpawner.h"
#include "EngineUtils.h"
#include "POEMonster_Base.h"
#include "Engine/AssetManager.h"
#include "ActorObjectPool.h"
#include "POEGrux_Boss.h"

void UPOEStageStartWidget::OnStageStart()
{
	UWorld* CurrentWorld = GetWorld();

	FSoftObjectPath SlotClassPath(TEXT("/Game/POE/Blueprints/Actor/POEMonster_Normal1.POEMonster_Normal1_C"));

	int SpawnCount_NormalMonster = 2;
	int SpawnCount_SpawnPoint = 0;
	TSubclassOf<APOEMonster_Base> NormalMonster = UAssetManager::GetStreamableManager().LoadSynchronous(TSoftClassPtr<APOEMonster_Base>(SlotClassPath));

	for (TActorIterator<AMonsterSpawner> It(CurrentWorld); It; ++It) {
		AMonsterSpawner* SpawnPoint = *It;
		FVector SpawnLocation = SpawnPoint->GetSpawnLocation();

		if (SpawnPoint->IsBossSpawner) {
			if(GameInstance->CurStageLevel == 10) GameInstance->ReadyBoss(EBossType::GRUX, SpawnLocation);
			continue;
		}

		SpawnCount_SpawnPoint++;
		for (int i = 0; i < SpawnCount_NormalMonster; i++) {
			FRandomStream Random(FMath::Rand());
			int16 RandX = -20 + 20 * i;
			int16 RandY = 20 - FMath::Sqrt(pow(RandX, 2.0));

			SpawnLocation.X += RandX;
			SpawnLocation.Y += RandY;

			SpawnNormalMonster(NormalMonster->GetDefaultObject()->GetClass(), SpawnLocation);
		}
	}

	CHECKRETURN(SpawnCount_SpawnPoint == 0);
	GameInstance->SetCountSpawnMonster(SpawnCount_NormalMonster * SpawnCount_SpawnPoint + (GameInstance->CurStageLevel == 10? 1 : 0));
	GameInstance->IsDoingBattle = true;
	OnCancel();
}

void UPOEStageStartWidget::OnCancel()
{
	RemoveFromParent();
	UPOEGameInstance* GameInstance = Cast<UPOEGameInstance>(GetWorld()->GetGameInstance());
	if (GameInstance != nullptr) {
		UUserWidget* InventoryWidget = GameInstance->UIScreenInteraction->GetPanel(EUIPanelName::INVENTORY);
		if (InventoryWidget != nullptr) InventoryWidget->RemoveFromParent();

		UUserWidget* HUDPanel = GameInstance->UIScreenInteraction->GetPanel(EUIPanelName::HUD);
		HUDPanel->SetVisibility(ESlateVisibility::Visible);
	}
	GetOwningPlayer()->SetPause(false);
}

void UPOEStageStartWidget::NativeConstruct() {
	Super::NativeConstruct();

	UButton* StartButton = Cast<UButton>(GetWidgetFromName(TEXT("Btn_Start")));
	CHECKRETURN(StartButton == nullptr);
	StartButton->OnClicked.AddDynamic(this, &UPOEStageStartWidget::OnStageStart);

	UButton* CancelButton = Cast<UButton>(GetWidgetFromName(TEXT("Btn_Cancel")));
	CHECKRETURN(CancelButton == nullptr);
	CancelButton->OnClicked.AddDynamic(this, &UPOEStageStartWidget::OnCancel);

	PrevStageButton = Cast<UButton>(GetWidgetFromName(TEXT("Btn_PrevStage")));
	CHECKRETURN(PrevStageButton == nullptr);
	PrevStageButton->OnClicked.AddDynamic(this, &UPOEStageStartWidget::PrevStage);

	NextStageButton = Cast<UButton>(GetWidgetFromName(TEXT("Btn_NextStage")));
	CHECKRETURN(NextStageButton == nullptr);
	NextStageButton->OnClicked.AddDynamic(this, &UPOEStageStartWidget::NextStage);

	CurrentStageNumber = Cast<UTextBlock>(GetWidgetFromName(TEXT("Text_StageNumber")));
	CHECKRETURN(CurrentStageNumber == nullptr);

	GameInstance = Cast<UPOEGameInstance>(GetWorld()->GetGameInstance());
	CHECKRETURN(GameInstance == nullptr);

	SetStage();
}

void UPOEStageStartWidget::PrevStage()
{
	GameInstance->CurStageLevel--;
	SetStage();
}

void UPOEStageStartWidget::NextStage()
{
	GameInstance->CurStageLevel++;
	SetStage();
}

void UPOEStageStartWidget::SetStage()
{
	if (GameInstance->CurStageLevel < 1) GameInstance->CurStageLevel = 1;
	else if (GameInstance->CurStageLevel > GameInstance->MaxStageLevel) GameInstance->CurStageLevel = GameInstance->MaxStageLevel;

	PrevStageButton->SetVisibility(ESlateVisibility::Visible);
	NextStageButton->SetVisibility(ESlateVisibility::Visible);

	if (GameInstance->CurStageLevel == 1) {
		PrevStageButton->SetVisibility(ESlateVisibility::Hidden);
	}
	if (GameInstance->CurStageLevel == GameInstance->MaxStageLevel) {
		NextStageButton->SetVisibility(ESlateVisibility::Hidden);
	}

	CurrentStageNumber->SetText(FText::AsNumber(GameInstance->CurStageLevel));
}

void UPOEStageStartWidget::SpawnNormalMonster(UClass * MonsterClass, FVector Location)
{
	FPOEMonsterStatData* MonsterStatData = GameInstance->GetMonsterDataForId(1);
	CHECKRETURN(MonsterStatData == nullptr);

	APOEMonster_Base* Monster = GameInstance->MonsterPooling->GetUnUseMonster(MonsterStatData->MonsterId);
	if (Monster == nullptr) {
		Monster = GetWorld()->SpawnActor<APOEMonster_Base>(MonsterClass, Location, FRotator::ZeroRotator);
		CHECKRETURN(Monster == nullptr);

		Monster->MonsterId = MonsterStatData->MonsterId;
		GameInstance->MonsterPooling->AddMonster(Monster);
	}

	CHECKRETURN(Monster == nullptr);
	Monster->SetActorLocation(Location);
	Monster->Active();
	Monster->CharacterStatus->InitAttackValue(MonsterStatData->AttackValue * GameInstance->CurStageLevel);
	Monster->CharacterStatus->InitHPVale(MonsterStatData->HpValue * GameInstance->CurStageLevel);
}
