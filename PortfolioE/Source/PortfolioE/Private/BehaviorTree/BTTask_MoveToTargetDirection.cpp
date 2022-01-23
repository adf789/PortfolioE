// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_MoveToTargetDirection.h"
#include "POEMonster_Base.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "AIController.h"


UBTTask_MoveToTargetDirection::UBTTask_MoveToTargetDirection() {
	NodeName = TEXT("MoveToTargetDirection");
	bNotifyTick = true;
}

EBTNodeResult::Type UBTTask_MoveToTargetDirection::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) {
	EBTNodeResult::Type ResultType = Super::ExecuteTask(OwnerComp, NodeMemory);

	Target = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(BlackboardKey.SelectedKeyName));
	CHECKRETURN(Target == nullptr, EBTNodeResult::Failed);

	APOEMonster_Base* ControllingPawn = Cast<APOEMonster_Base>(OwnerComp.GetAIOwner()->GetPawn());
	CHECKRETURN(ControllingPawn == nullptr, EBTNodeResult::Failed);

	IsSetDestination = true;
	StopDistanceMax = ControllingPawn->GetAttackDistance();
	return EBTNodeResult::InProgress;
}

void UBTTask_MoveToTargetDirection::TickTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	if (IsSetDestination) {
		FVector TargetLocation = Target->GetActorLocation();
		float TargetDistance = FVector::Distance(TargetLocation, OwnerComp.GetAIOwner()->GetPawn()->GetActorLocation());

		if (TargetDistance <= StopDistanceMax) {
			IsSetDestination = false;
			UNavigationSystem::SimpleMoveToLocation(OwnerComp.GetAIOwner(), OwnerComp.GetAIOwner()->GetPawn()->GetActorLocation());
			return;
		}
		else {
			UNavigationSystem::SimpleMoveToLocation(OwnerComp.GetAIOwner(), TargetLocation);
		}
	}
	else {
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}