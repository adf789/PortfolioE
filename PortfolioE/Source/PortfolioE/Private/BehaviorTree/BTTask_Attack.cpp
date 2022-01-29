// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_Attack.h"
#include "POEMonster_Base.h"
#include "POEMonsterAIController.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_Attack::UBTTask_Attack() {
	NodeName = TEXT("Attack");
	bNotifyTick = true;
}

EBTNodeResult::Type UBTTask_Attack::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	APOECharacter_Base* Attacker = Cast<APOEMonster_Base>(OwnerComp.GetAIOwner()->GetPawn());

	if (Attacker == nullptr) {
		UE_LOG(POE, Error, TEXT("Attacker is null"));
		return EBTNodeResult::Failed;
	}

	if (Attacker->GetState() == ECharacterBehaviorState::ATTACKING) return EBTNodeResult::InProgress;

	UNavigationSystem::SimpleMoveToLocation(OwnerComp.GetAIOwner(), Attacker->GetActorLocation());
	Attacker->Attack();
	return EBTNodeResult::InProgress;
}

void UBTTask_Attack::TickTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);
	APOECharacter_Base* Attacker = Cast<APOECharacter_Base>(OwnerComp.GetAIOwner()->GetPawn());
	if (Attacker == nullptr) return FinishLatentTask(OwnerComp, EBTNodeResult::Failed);


	if (Attacker->GetState() != ECharacterBehaviorState::ATTACKING) {
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}
