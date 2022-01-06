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
	DAMAGE_TEXT UMETA(DisplayName = "DAMAGE_TEXT"),
	MENU UMETA(DisplayName = "ViewportLevel MENU"),
	NORMAL_PANEL UMETA(DisplayName = "ViewportLevel NORMAL_PANEL"),
};
