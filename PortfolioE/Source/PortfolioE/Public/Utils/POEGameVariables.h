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
	MENU = 1 UMETA(DisplayName = "ViewportLevel MENU"),
	NORMAL_PANEL UMETA(DisplayName = "ViewportLevel NORMAL_PANEL"),
};
