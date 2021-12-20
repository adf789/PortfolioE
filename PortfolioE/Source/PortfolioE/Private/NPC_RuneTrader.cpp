// Fill out your copyright notice in the Description page of Project Settings.

#include "NPC_RuneTrader.h"
#include "Components/WidgetComponent.h"

ANPC_RuneTrader::ANPC_RuneTrader() {
	/*static ConstructorHelpers::FObjectFinder<USkeletalMesh>
		SK_MODEL(TEXT("/Game/InfinityBladeWarriors/Character/CompleteCharacters/SK_CharM_Forge.SK_CharM_Forge"));
	if (SK_MODEL.Succeeded()) {
		GetMesh()->SetSkeletalMesh(SK_MODEL.Object);
	}*/
}

void ANPC_RuneTrader::PostInitializeComponents()
{
}

void ANPC_RuneTrader::OnTalkButtonClicked()
{
}

void ANPC_RuneTrader::OnTradeButtonClicked()
{
}
