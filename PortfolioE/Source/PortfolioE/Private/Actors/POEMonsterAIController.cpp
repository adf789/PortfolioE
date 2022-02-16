// Fill out your copyright notice in the Description page of Project Settings.

#include "POEMonsterAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"

const FName APOEMonsterAIController::BBKEY_Target(TEXT("Target"));
const FName APOEMonsterAIController::BBKEY_TargetDirection(TEXT("TargetDirection"));
const FName APOEMonsterAIController::BBKEY_AttackDist(TEXT("AttackDist"));
const FName APOEMonsterAIController::BBKEY_WaypointLocation(TEXT("WaypointLocation"));
const FName APOEMonsterAIController::BBKEY_SetWaypointDirection(TEXT("SetWaypointLocation"));
const FName APOEMonsterAIController::BBKEY_IsRunMonster(TEXT("IsRunMonster"));

APOEMonsterAIController::APOEMonsterAIController() {
	LoadBehaviorTree();
}

void APOEMonsterAIController::Possess(APawn * InPawn)
{
	Super::Possess(InPawn);
}

void APOEMonsterAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APOEMonsterAIController::RunAI()
{
	if (UseBlackboard(BBDatas, Blackboard)) {
		if (!RunBehaviorTree(BTPattern)) {
			UE_LOG(POE, Error, TEXT("Couldn't run Behavior"));
		}
	}
	if (OnPostRunAI.IsBound()) OnPostRunAI.Broadcast();
}

void APOEMonsterAIController::StopAI()
{
	UBehaviorTreeComponent* BehaviorTreeComponent = Cast<UBehaviorTreeComponent>(BrainComponent);
	if (BehaviorTreeComponent != nullptr) {
		BehaviorTreeComponent->StopTree(EBTStopMode::Safe);
	}
}

void APOEMonsterAIController::LoadBehaviorTree()
{
	static ConstructorHelpers::FObjectFinder<UBehaviorTree>
		BTObject(TEXT("/Game/POE/AI/BT_POEMonster.BT_POEMonster"));
	if (BTObject.Succeeded()) {
		BTPattern = BTObject.Object;
	}

	static ConstructorHelpers::FObjectFinder<UBlackboardData>
		BBObject(TEXT("/Game/POE/AI/BB_POEMonster.BB_POEMonster"));
	if (BBObject.Succeeded()) {
		BBDatas = BBObject.Object;
	}
}
