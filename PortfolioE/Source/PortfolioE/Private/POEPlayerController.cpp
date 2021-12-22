// Fill out your copyright notice in the Description page of Project Settings.

#include "POEPlayerController.h"
#include "POECharacter.h"
#include "POENpcCharacter.h"
#include "POENpcMenuWidget.h"

APOEPlayerController::APOEPlayerController()
{
	static ConstructorHelpers::FClassFinder<UPOENpcMenuWidget>
		UI_MENU_C(TEXT("/Game/POE/UIWidget/UI_MenuWidget.UI_MenuWidget_c"));
	if (UI_MENU_C.Succeeded()) {
		MenuWidgetClass = UI_MENU_C.Class;
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

//void APOEPlayerController::PlayerTick(float DeltaTime)
//{
//	//DetectNPCOnCursor();
//}

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
			screenPos.Y -= 110.0f;

			npcMenuWidget = CreateWidget<UPOENpcMenuWidget>(this, MenuWidgetClass);
			npcMenuWidget->AddToViewport(EViewportLevel::MENU);
			npcMenuWidget->SetPositionInViewport(screenPos);

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
	npcMenuWidget->OnTrade.AddUObject(npc, &APOENpcCharacter::OnTrade);
}
