// Fill out your copyright notice in the Description page of Project Settings.

#include "BTService_Detect.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "POEMonsterAIController.h"

UBTService_Detect::UBTService_Detect() {
	NodeName = TEXT("Detect");
}

void UBTService_Detect::TickNode(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	APawn* ControlPawn = OwnerComp.GetAIOwner()->GetPawn();
	CHECKRETURN(ControlPawn == nullptr);

	TArray<FOverlapResult> OverlapResults;
	FCollisionQueryParams ConllisionQueryParams(NAME_None, false, ControlPawn);
	bool bResult = GetWorld()->OverlapMultiByChannel(
		OverlapResults,
		ControlPawn->GetActorLocation(),
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel2,
		FCollisionShape::MakeSphere(600.0f),
		ConllisionQueryParams
	);

	if (bResult) {
		/*OwnerComp.GetBlackboardComponent()*/
	}
}
