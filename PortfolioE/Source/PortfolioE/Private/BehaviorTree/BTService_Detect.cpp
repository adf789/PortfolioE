// Fill out your copyright notice in the Description page of Project Settings.

#include "BTService_Detect.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "POEMonsterAIController.h"
#include "DrawDebugHelpers.h"
#include "POECharacter.h"

UBTService_Detect::UBTService_Detect() {
	NodeName = TEXT("Detect");
}

void UBTService_Detect::TickNode(UBehaviorTreeComponent& OwnerComp, uint8 * NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	APOECharacter_Base* ControlPawn = Cast<APOECharacter_Base>(OwnerComp.GetAIOwner()->GetPawn());
	CHECKRETURN(ControlPawn == nullptr);

	UWorld* World = GetWorld();
	FVector Center = ControlPawn->GetActorLocation();
	float DetectedRange = ControlPawn->GetAIDetectRange();

	TArray<FOverlapResult> OverlapResults;
	FCollisionQueryParams ConllisionQueryParams(NAME_None, false, ControlPawn);
	bool bResult = World->OverlapMultiByChannel(
		OverlapResults,
		ControlPawn->GetActorLocation(),
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel2,
		FCollisionShape::MakeSphere(DetectedRange),
		ConllisionQueryParams
	);

	if (bResult) {
		OwnerComp.GetBlackboardComponent()->SetValueAsObject(APOEMonsterAIController::BBKEY_Target, nullptr);
		for (FOverlapResult const& OverlapResult : OverlapResults) {
			APOECharacter* Character = Cast<APOECharacter>(OverlapResult.GetActor());

			if (Character != nullptr) {
				//DrawDebugSphere(World, Center, DetectedRange, 16, FColor::Green, false, .2f);

				OwnerComp.GetBlackboardComponent()->SetValueAsObject(APOEMonsterAIController::BBKEY_Target, Character);
				return;
			}
		}		
	}

	OwnerComp.GetBlackboardComponent()->SetValueAsObject(APOEMonsterAIController::BBKEY_Target, nullptr);
	//DrawDebugSphere(World, Center, DetectedRange, 16, FColor::Red, false, .2f);
}
