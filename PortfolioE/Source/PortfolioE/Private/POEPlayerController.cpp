// Fill out your copyright notice in the Description page of Project Settings.

#include "POEPlayerController.h"
#include "POECharacter.h"

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
		if (currentTarget != hitResult.Component) {
			if (currentTarget != nullptr) currentTarget->SetRenderCustomDepth(false);
		}

		AActor* actor = hitResult.GetActor();
		if (actor != nullptr) {
			if (actor->Tags.Contains(TEXT("NPC"))) {
				currentTarget = hitResult.Component;
				hitResult.Component->SetRenderCustomDepth(true);
			}
		}
	}
	else {
		if (currentTarget != nullptr) currentTarget->SetRenderCustomDepth(false);
	}
}
