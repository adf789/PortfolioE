// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_AroundTo.h"
#include "AIController.h"
#include "POEMonster_Base.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "DrawDebugHelpers.h"

UBTTask_AroundTo::UBTTask_AroundTo() {
	NodeName = TEXT("AroundToTarget");
	//bNotifyTick = true;
	IsSetDistance = false;
}

EBTNodeResult::Type UBTTask_AroundTo::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	EBTNodeResult::Type result = Super::ExecuteTask(OwnerComp, NodeMemory);

	AActor* Target = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(BlackboardKey.SelectedKeyName));
	CHECKRETURN(Target == nullptr, EBTNodeResult::Failed);

	float RandomXDistance = MaxMovableRadius;
	APOEMonster_Base* ControllingPawn = Cast<APOEMonster_Base>(OwnerComp.GetAIOwner()->GetPawn());
	CHECKRETURN(ControllingPawn == nullptr, EBTNodeResult::Failed);
	/*if (ControllingPawn == nullptr) RandomXDistance = 1;
	else RandomXDistance = ControllingPawn->GetAttackDistance();*/

	FVector TargetLocation = Target->GetActorLocation();

	FNavLocation ResultLocation;
	UNavigationSystem* NaviSystem = GetWorld()->GetNavigationSystem();
	CHECKRETURN(NaviSystem == nullptr, EBTNodeResult::Failed);
	bool bResult = NaviSystem->GetRandomPointInNavigableRadius(TargetLocation, RandomXDistance, ResultLocation);
	CHECKRETURN(!bResult, EBTNodeResult::Failed);

	FinalToLocation = ResultLocation.Location;
	//FinalToLocation.Z = ControllingPawn->GetActorLocation().Z;
	UNavigationSystem::SimpleMoveToLocation(OwnerComp.GetAIOwner(), FinalToLocation);
	//IsSetDistance = true;
	return EBTNodeResult::Succeeded;
}

void UBTTask_AroundTo::TickTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	if (IsSetDistance) {
		float DestinationDistance = FVector::Dist(OwnerComp.GetAIOwner()->GetPawn()->GetActorLocation(), FinalToLocation);
		if (DestinationDistance <= 80.0f) IsSetDistance = false;
	}else {
		IsSetDistance = false;
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}
