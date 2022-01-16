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
	NORMAL_PANEL UMETA(DisplayName = "ViewportLevel NORMAL_PANEL"),
};

UENUM()
enum ECharacterBehaviorState {
	IDLE UMETA(DisplayName = "CharacterBehaviorState IDLE"),
	ATTACKING UMETA(DisplayName = "CharacterBehaviorState ATTACKING"),
	HITTING UMETA(DisplayName = "CharacterBehaviorState HITTING"),
	WALKING UMETA(DisplayName = "CharacterBehaviorState WALKING"),
};
