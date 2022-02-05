// Fill out your copyright notice in the Description page of Project Settings.

#include "POEPlayerController.h"
#include "POECharacter.h"
#include "POENpcCharacter.h"
#include "POENpcMenuWidget.h"
#include "Components/WidgetComponent.h"
#include "POEPlayerHUDWidget.h"

APOEPlayerController::APOEPlayerController()
{
	static ConstructorHelpers::FClassFinder<UPOENpcMenuWidget>
		UI_MENU_C(TEXT("/Game/POE/UIWidget/UI_MenuWidget.UI_MenuWidget_c"));
	if (UI_MENU_C.Succeeded()) {
		menuWidgetClass = UI_MENU_C.Class;
	}

	static ConstructorHelpers::FClassFinder<UUserWidget>
		UI_HUD_C(TEXT("/Game/POE/UIWidget/UI_PlayerHUDWidget.UI_PlayerHUDWidget_c"));
	if (UI_HUD_C.Succeeded()) {
		HUDWidgetClass = UI_HUD_C.Class;

	}
}

void APOEPlayerController::SetupInputComponent() {
	Super::SetupInputComponent();

}

void APOEPlayerController::BeginPlay()
{
	Super::BeginPlay();
	bShowMouseCursor = true;
	
	GetWorldTimerManager().SetTimer(detectTimer, this, &APOEPlayerController::DetectNPCOnCursor, .1f, true);
}

void APOEPlayerController::DetectNPCOnCursor()
{
	FHitResult hitResult;
	bool bResult = GetHitResultUnderCursor(ECollisionChannel::ECC_WorldDynamic, true, hitResult);
	if (bResult) {
		AActor* actor = hitResult.GetActor();
		if (actor == nullptr || !actor->Tags.Contains(TEXT("NPC"))) {
			if (outlineTarget != nullptr) {
				outlineTarget->SetRenderCustomDepth(false);
				outlineTarget = nullptr;
			}
			return;
		}

		ACharacter* character = Cast<ACharacter>(actor);
		if (outlineTarget != nullptr) {
			if (outlineTarget != character->GetMesh()) {
				outlineTarget->SetRenderCustomDepth(false);
			}
		}
		outlineTarget = character->GetMesh();
		outlineTarget->SetRenderCustomDepth(true);
	}
	else {
		if (outlineTarget != nullptr) {
			outlineTarget->SetRenderCustomDepth(false);
			outlineTarget = nullptr;
		}
	}
}

bool APOEPlayerController::IsDetectedNPC()
{
	return outlineTarget != nullptr;
}

void APOEPlayerController::ShowNpcMenuWidget(APOENpcCharacter * npc)
{
	FVector2D screenPos;
	bool bResult = ProjectWorldLocationToScreen(npc->GetActorLocation(), screenPos);
	if (bResult) {
		if (npcMenuWidget == nullptr) {
			screenPos.Y -= 140.0f;

			npcMenuWidget = CreateWidget<UPOENpcMenuWidget>(this, menuWidgetClass);
			npcMenuWidget->AddToViewport(EViewportLevel::MENU);
			npcMenuWidget->SetPositionInViewport(screenPos);
			npcMenuWidget->SetDesiredSizeInViewport(FVector2D(150, 200));

			BindNpcMenuAction(npc);
		}
	}
}

void APOEPlayerController::HideNpcMenuWidget()
{
	if (npcMenuWidget == nullptr) return;
	npcMenuWidget->RemoveFromParent();
	npcMenuWidget = nullptr;
}

void APOEPlayerController::BindNpcMenuAction(APOENpcCharacter * npc)
{
	if (npcMenuWidget == nullptr || npc == nullptr) return;
	npcMenuWidget->OnTalk.AddUObject(npc, &APOENpcCharacter::OnTalk);
	npcMenuWidget->OnAction.AddUObject(npc, &APOENpcCharacter::OnAction);
	npcMenuWidget->OnCancel.AddUObject(npc, &APOENpcCharacter::OnCancel);
}

UUserWidget * APOEPlayerController::ShowMenuWidget(TSubclassOf<UUserWidget> WidgetClass, EViewportLevel Level, FVector Location)
{
	FVector2D ScreenPos;
	UUserWidget* TempWidget = CreateWidget<UUserWidget>(this, WidgetClass);
	bool bResult = ProjectWorldLocationToScreen(Location, ScreenPos);
	if (bResult) {
		TempWidget->AddToViewport(EViewportLevel::MENU);
		TempWidget->SetPositionInViewport(ScreenPos);

		return TempWidget;
	}
	else {
		return nullptr;
	}
}

UPOEPlayerHUDWidget* APOEPlayerController::CreateAndInitHUDWidget(APOECharacter_Base * Character_Base)
{
	UPOEPlayerHUDWidget* HUDWidget = CreateWidget<UPOEPlayerHUDWidget>(this, HUDWidgetClass);
	HUDWidget->AddToViewport(EViewportLevel::HUD);

	if (Character_Base != nullptr) {
		HUDWidget->BindCharacterStat(Character_Base->CharacterStatus);
	}

	return HUDWidget;
}

void APOEPlayerController::Possess(APawn * aPawn)
{
	Super::Possess(aPawn);

	APOECharacter_Base* Character_Base = Cast<APOECharacter_Base>(aPawn);
	HUDWidget = CreateAndInitHUDWidget(Character_Base);
}
