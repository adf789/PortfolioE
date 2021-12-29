// Fill out your copyright notice in the Description page of Project Settings.

#include "POEMonsterAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"

APOEMonsterAIController::APOEMonsterAIController() {
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


