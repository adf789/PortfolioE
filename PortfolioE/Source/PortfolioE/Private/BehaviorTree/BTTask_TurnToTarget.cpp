// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_TurnToTarget.h"
#include "POEMonsterAIController.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_TurnToTarget::UBTTask_TurnToTarget() {
	NodeName = TEXT("Look Target");
}

EBTNodeResult::Type UBTTask_TurnToTarget::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	AActor* Target = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(APOEMonsterAIController::BBKEY_Target));
	CHECKRETURN(Target == nullptr, EBTNodeResult::Failed);

	AActor* Monster = Cast<AActor>(OwnerComp.GetAIOwner()->GetPawn());
	CHECKRETURN(Monster == nullptr, EBTNodeResult::Failed);

	FVector LookVector = Target->GetActorLocation() - Monster->GetActorLocation();
	LookVector.Z = .0f;
	FRotator LookRot = FRotationMatrix::MakeFromX(LookVector).Rotator();
	Monster->SetActorRotation(FMath::RInterpTo(Monster->GetActorRotation(), LookRot, GetWorld()->GetDeltaSeconds(), 2.0f));

	return EBTNodeResult::Succeeded;
}
