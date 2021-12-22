// Fill out your copyright notice in the Description page of Project Settings.

#include "NPC_RuneTrader.h"
#include "POERuneStoreWidget.h"
#include "Components/WidgetComponent.h"

ANPC_RuneTrader::ANPC_RuneTrader() : Super() {
	static ConstructorHelpers::FObjectFinder<USkeletalMesh>
		SK_MODEL(TEXT("/Game/InfinityBladeWarriors/Character/CompleteCharacters/SK_CharM_Forge.SK_CharM_Forge"));
	if (SK_MODEL.Succeeded()) {
		GetMesh()->SetSkeletalMesh(SK_MODEL.Object);
		GetMesh()->SetRelativeLocation(FVector(.0f, .0f, -90.0f));
	}

	static ConstructorHelpers::FClassFinder<UUserWidget>
		UI_RUNE_STORE_C(TEXT("/Game/POE/UIWidget/UI_RuneStorePanel.UI_RuneStorePanel_c"));
	if (UI_RUNE_STORE_C.Succeeded()) {
		this->storeUIClass = UI_RUNE_STORE_C.Class;
	}
}

void ANPC_RuneTrader::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

void ANPC_RuneTrader::OnTalk()
{
	Super::OnTalk();
	TEST_LOG("On Talk");
}

void ANPC_RuneTrader::OnTrade()
{
	Super::OnTrade();
	TEST_LOG("On Trade");

	runeStoreWidget = CreateWidget<UPOERuneStoreWidget>(GetWorld()->GetFirstPlayerController(), storeUIClass);
	runeStoreWidget->AddToViewport(EViewportLevel::NORMAL_PANEL);
}
