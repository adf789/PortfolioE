// Fill out your copyright notice in the Description page of Project Settings.

#include "UIScreenInteraction.h"
#include "UserWidget.h"
#include "POEUIClassPaths.h"


// Sets default values for this component's properties
UUIScreenInteraction::UUIScreenInteraction()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UUIScreenInteraction::BeginPlay()
{
	Super::BeginPlay();
}

void UUIScreenInteraction::DestroyComponent(bool bPromoteChildren)
{
	Super::DestroyComponent(bPromoteChildren);
	UIPanels.Reset();
}

void UUIScreenInteraction::ShowPanel(EUIPanelName ScreenName)
{
	FString ScreenNameStr;
	const UEnum* enumPtr = FindObject<UEnum>(ANY_PACKAGE, TEXT("EUIPanelName"), true);
	if (enumPtr) {
		ScreenNameStr = enumPtr->GetNameStringByIndex((int8)ScreenName);
	}

	if (ScreenNameStr.Equals(TEXT(""))) return;

	TEST_LOG_WITH_VAR("UI Count: %d", GetDefault<UPOEUIClassPaths>()->UIClassPaths.Num());
	if (GetDefault<UPOEUIClassPaths>()->UIClassPaths.Num() > (int8)ScreenName) {
		TEST_LOG_WITH_VAR("Show: %s", *GetDefault<UPOEUIClassPaths>()->UIClassPaths[(int8)ScreenName].GetAssetPathString());
	}
	else {
		TEST_LOG_WITH_VAR("Invalid: %s", *ScreenNameStr);
	}
}

void UUIScreenInteraction::ClosePanel(EUIPanelName ScreenName)
{
	if (!UIPanels.Contains(ScreenName)) return;

	UUserWidget* TargetPanel = nullptr;
	if (UIPanels.RemoveAndCopyValue(ScreenName, TargetPanel)) {
		if (TargetPanel != nullptr) TargetPanel->RemoveFromParent();
	}
}

UUserWidget * UUIScreenInteraction::GetPanel(EUIPanelName ScreenName)
{
	if (UIPanels.Contains(ScreenName)) return UIPanels[ScreenName];
	return nullptr;
}

