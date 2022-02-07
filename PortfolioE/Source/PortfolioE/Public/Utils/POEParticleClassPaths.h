// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "POEParticleClassPaths.generated.h"

/**
 * 
 */
UCLASS(config= ParticlePath)
class PORTFOLIOE_API UPOEParticleClassPaths : public UObject
{
	GENERATED_BODY()
	
public:
	UPROPERTY(config)
	TArray<FSoftObjectPath> ParticlePaths;
};
