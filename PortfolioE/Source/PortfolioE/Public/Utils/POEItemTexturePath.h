// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "PortfolioE.h"
#include "UObject/NoExportTypes.h"
#include "POEItemTexturePath.generated.h"

/**
 * 
 */
UCLASS(config = ItemPath)
class PORTFOLIOE_API UPOEItemTexturePath : public UObject
{
	GENERATED_BODY()
	
public:
	UPOEItemTexturePath();

	UPROPERTY(config)
	TArray<FSoftObjectPath> TexturePaths;
};
