// Fill out your copyright notice in the Description page of Project Settings.

#include "UIScreenInteraction.h"
#include "UserWidget.h"
#include "POEUIClassPaths.h"
#include "Components/Widget.h"
#include "POEGameInstance.h"
#include "Engine/AssetManager.h"
#include "POECharacter.h"

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
	if (GetDefault<UPOEUIClassPaths>()->UIClassPaths.Num() > (int8)ScreenName) {
		FSoftObjectPath UIClassPath = GetDefault<UPOEUIClassPaths>()->UIClassPaths[(int8)ScreenName];

		TSubclassOf<UUserWidget> UIWidgetClass = UAssetManager::GetStreamableManager().LoadSynchronous(TSoftClassPtr<UUserWidget>(UIClassPath));
		CHECKRETURN(UIWidgetClass.Get() == nullptr);
		
		UUserWidget* UIWidget = CreateWidget<UUserWidget>(GetWorld()->GetFirstPlayerController(), UIWidgetClass);
		UIWidget->AddToViewport(EViewportLevel::NORMAL_PANEL);
		UIPanels.Add(ScreenName, UIWidget);
	}
	else {
		FString ScreenNameStr;
		const UEnum* enumPtr = FindObject<UEnum>(ANY_PACKAGE, TEXT("EUIPanelName"), true);
		if (enumPtr) {
			ScreenNameStr = enumPtr->GetNameStringByIndex((int8)ScreenName);
			TEST_LOG_WITH_VAR("Invalid: %s", *ScreenNameStr);
		}
		else TEST_LOG_WITH_VAR("Invalid");
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

int UUIScreenInteraction::PanelCount()
{
	return UIPanels.Num();
}

