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
	MENU UMETA(DisplayName = "ViewportLevel MENU"),
	HUD UMETA(DisplayName = "ViewportLevel HUD"),
	NORMAL_PANEL UMETA(DisplayName = "ViewportLevel NORMAL_PANEL"),
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
};

UENUM(BlueprintType)
enum class EItemType : uint8 {
	EQUIPMENT,
	ETC,
};
