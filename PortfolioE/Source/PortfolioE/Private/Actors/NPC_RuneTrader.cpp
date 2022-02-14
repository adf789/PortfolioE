// Fill out your copyright notice in the Description page of Project Settings.

#include "NPC_RuneTrader.h"
#include "POEStageStartWidget.h"
#include "Components/WidgetComponent.h"
#include "POEPlayerController.h"
#include "POECharacter.h"
#include "POEInventoryAndEquipWidget.h"

ANPC_RuneTrader::ANPC_RuneTrader() {
	static ConstructorHelpers::FObjectFinder<USkeletalMesh>
		SK_MODEL(TEXT("/Game/InfinityBladeWarriors/Character/CompleteCharacters/SK_CharM_Forge.SK_CharM_Forge"));
	if (SK_MODEL.Succeeded()) {
		GetMesh()->SetSkeletalMesh(SK_MODEL.Object);
		GetMesh()->SetRelativeLocation(FVector(.0f, .0f, -90.0f));
	}

	static ConstructorHelpers::FClassFinder<UUserWidget>
		UI_STAGE_START_C(TEXT("/Game/POE/UIWidget/UI_StageStartPanel.UI_StageStartPanel_c"));
	if (UI_STAGE_START_C.Succeeded()) {
		stageUIClass = UI_STAGE_START_C.Class;
	}

	static ConstructorHelpers::FClassFinder<UUserWidget>
		UI_INVENTORY_EQUIP_C(TEXT("/Game/POE/UIWidget/UI_InventoryAndEquipWidget.UI_InventoryAndEquipWidget_C"));
	if (UI_INVENTORY_EQUIP_C.Succeeded()) {
		InventoryUIClass = UI_INVENTORY_EQUIP_C.Class;
	}
}

void ANPC_RuneTrader::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

void ANPC_RuneTrader::OnTalk()
{
	Super::OnTalk();

	GetWorld()->GetFirstPlayerController()->SetPause(true);

	auto POEPlayerController = Cast<APOEPlayerController>(GetWorld()->GetFirstPlayerController());
	CHECKRETURN(POEPlayerController == nullptr);

	POEPlayerController->HideNpcMenuWidget();

	APOECharacter* Player = Cast<APOECharacter>(POEPlayerController->GetPawn());
	CHECKRETURN(Player == nullptr);

	Player->UIScreens->ShowPanel(EUIPanelName::LOTTERY);
}

void ANPC_RuneTrader::OnAction()
{
	Super::OnAction();

	GetWorld()->GetFirstPlayerController()->SetPause(true);

	auto POEPlayerController = Cast<APOEPlayerController>(GetWorld()->GetFirstPlayerController());
	CHECKRETURN(POEPlayerController == nullptr);

	POEPlayerController->HideNpcMenuWidget();

	APOECharacter* Player = Cast<APOECharacter>(POEPlayerController->GetPawn());
	CHECKRETURN(Player == nullptr);

	Player->UIScreens->ShowPanel(EUIPanelName::INVENTORY);
	Player->UIScreens->ShowPanel(EUIPanelName::STAGE_START);

	UUserWidget* HUDPanel = Player->UIScreens->GetPanel(EUIPanelName::HUD);
	HUDPanel->SetVisibility(ESlateVisibility::Hidden);

	UPOEInventoryAndEquipWidget* InventoryAndEquipWidget = Cast<UPOEInventoryAndEquipWidget>(Player->UIScreens->GetPanel(EUIPanelName::INVENTORY));
	if(InventoryAndEquipWidget != nullptr) InventoryAndEquipWidget->InitInventoryView();
}

void ANPC_RuneTrader::OnCancel()
{
	Super::OnCancel();
}
