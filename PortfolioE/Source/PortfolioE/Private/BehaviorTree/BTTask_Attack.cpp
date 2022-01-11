// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_Attack.h"
#include "POECharacter_Base.h"
#include "POEMonsterAIController.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_Attack::UBTTask_Attack() {
	NodeName = TEXT("Attack");
}

EBTNodeResult::Type UBTTask_Attack::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	APOECharacter_Base* Attacker = Cast<APOECharacter_Base>(OwnerComp.GetAIOwner()->GetPawn());

	if (Attacker == nullptr) {
		UE_LOG(POE, Error, TEXT("Attacker is null"));
		return EBTNodeResult::Failed;
	}
	
	if (!IsAttacking) {
		BTComponent = &OwnerComp;
		SetAttackState(true);
		Attacker->Attack();
		Attacker->OnAttackEndDelegate.AddLambda([this]() -> void {
			SetAttackState(false);
			});
	}
	return EBTNodeResult::Succeeded;
}

void UBTTask_Attack::TickTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);
	/*if (!IsAttacking) {
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}*/
}

void UBTTask_Attack::SetAttackState(bool Active)
{
	if(BTComponent != nullptr) BTComponent->GetBlackboardComponent()->SetValueAsFloat(APOEMonsterAIController::BBKEY_IsAttack, Active);
	IsAttacking = Active;
}
