// Fill out your copyright notice in the Description page of Project Settings.

#include "POECharacterHPWidget.h"
#include "Components/ProgressBar.h"
#include "POECharacterStat.h"

void UPOECharacterHPWidget::UpdateHPWidget()
{
	if (CharacterStat != nullptr && HpBar != nullptr) {
		HpBar->SetPercent(CharacterStat->GetHpRate());
	}
}

void UPOECharacterHPWidget::NativeConstruct()
{
	Super::NativeConstruct();

	HpBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("PB_HpBar")));
}

void UPOECharacterHPWidget::BindCharacterStat(UPOECharacterStat* CharacterStatComponent) {
	CharacterStat = CharacterStatComponent;
	CharacterStat->OnChangeHPAction.AddUObject(this, &UPOECharacterHPWidget::UpdateHPWidget);
}
