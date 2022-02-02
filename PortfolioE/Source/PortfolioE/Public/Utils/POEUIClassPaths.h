// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "PortfolioE.h"
#include "UObject/NoExportTypes.h"
#include "POEUIClassPaths.generated.h"

/**
 * 
 */
UCLASS(config = UIPath)
class PORTFOLIOE_API UPOEUIClassPaths : public UObject
{
	GENERATED_BODY()

public:
	UPOEUIClassPaths();
	
public:
	UPROPERTY(config)
	TArray<FSoftObjectPath> UIClassPaths;
};
