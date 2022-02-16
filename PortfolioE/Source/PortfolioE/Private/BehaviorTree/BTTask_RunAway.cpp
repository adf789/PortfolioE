// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_RunAway.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "POEMonsterAIController.h"

EBTNodeResult::Type UBTTask_RunAway::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) {
	AActor* Target = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(APOEMonsterAIController::BBKEY_Target));
	CHECKRETURN(Target == nullptr, EBTNodeResult::Failed);

	ACharacter* ControllingPawn = Cast<ACharacter>(OwnerComp.GetAIOwner()->GetPawn());
	CHECKRETURN(ControllingPawn == nullptr, EBTNodeResult::Failed);
	
	UNavigationSystem* NaviSystem = GetWorld()->GetNavigationSystem();
	CHECKRETURN(NaviSystem == nullptr, EBTNodeResult::Failed);

	if (!IsInit || FVector::Distance(ControllingPawn->GetActorLocation(), RandPoint.Location) < 150) {
		IsInit = true;
		bool bResult = NaviSystem->GetRandomPointInNavigableRadius(ControllingPawn->GetActorLocation(), 1000.0f, RandPoint);
		CHECKRETURN(!bResult, EBTNodeResult::Failed);
	}

	UNavigationSystem::SimpleMoveToLocation(OwnerComp.GetAIOwner(), RandPoint.Location);
	return EBTNodeResult::Succeeded;
}


