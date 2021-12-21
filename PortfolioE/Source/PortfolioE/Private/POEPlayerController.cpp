// Fill out your copyright notice in the Description page of Project Settings.

#include "POEPlayerController.h"
#include "POECharacter.h"
#include "POENpcCharacter.h"

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
