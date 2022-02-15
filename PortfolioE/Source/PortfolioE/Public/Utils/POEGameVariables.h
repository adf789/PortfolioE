// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"

/**
 * 
 */
class PORTFOLIOE_API POEGameVariables
{

};

UENUM()
enum EViewportLevel { 
	DAMAGE_TEXT UMETA(DisplayName = "ViewportLevel DAMAGE_TEXT"),
	HUD UMETA(DisplayName = "ViewportLevel HUD"),
	NORMAL_PANEL UMETA(DisplayName = "ViewportLevel NORMAL_PANEL"),
	MENU UMETA(DisplayName = "ViewportLevel MENU"),
};

UENUM(BlueprintType)
enum class ECharacterBehaviorState : uint8 {
	IDLE,
	ATTACKING,
	HITTING,
	DEAD,
};

UENUM(BlueprintType)
enum class EUIPanelName : uint8 {
	INVENTORY,
	STAGE_START,
	DETAIL_VIEW,
	HUD,
	LOTTERY,
	MERGE_CRAFT,
	MESSAGE_BOX,
	REWARD,
};

UENUM(BlueprintType)
enum class EItemType : uint8 {
	EQUIPMENT,
	ETC,
};

UENUM(BlueprintType)
enum class EBossType : uint8 {
	NONE,
	GRUX,
};
