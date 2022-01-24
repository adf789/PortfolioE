// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_MoveToTargetDirection.h"
#include "POEMonster_Base.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "POEMonsterAIController.h"


UBTTask_MoveToTargetDirection::UBTTask_MoveToTargetDirection() {
	NodeName = TEXT("MoveToTargetDirection");
}

EBTNodeResult::Type UBTTask_MoveToTargetDirection::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) {
	EBTNodeResult::Type ResultType = Super::ExecuteTask(OwnerComp, NodeMemory);

	AActor* Target = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(BlackboardKey.SelectedKeyName));
	CHECKRETURN(Target == nullptr, EBTNodeResult::Failed);

	int32 SetWaypointDirection = OwnerComp.GetBlackboardComponent()->GetValueAsInt(APOEMonsterAIController::BBKEY_SetWaypointDirection);
	if (SetWaypointDirection != 0) {
		FVector WaypointLocation = OwnerComp.GetBlackboardComponent()->GetValueAsVector(APOEMonsterAIController::BBKEY_WaypointLocation);

		float WaypointDistance = FVector::Distance(WaypointLocation, OwnerComp.GetAIOwner()->GetPawn()->GetActorLocation());

		if (WaypointDistance <= 50.0f) {
			OwnerComp.GetBlackboardComponent()->SetValueAsInt(APOEMonsterAIController::BBKEY_SetWaypointDirection, 0);
		}
		else {
			UNavigationSystem::SimpleMoveToLocation(OwnerComp.GetAIOwner(), WaypointLocation);
		}
	}
	else {
		UNavigationSystem::SimpleMoveToLocation(OwnerComp.GetAIOwner(), Target->GetActorLocation());
	}
	return EBTNodeResult::Succeeded;
}