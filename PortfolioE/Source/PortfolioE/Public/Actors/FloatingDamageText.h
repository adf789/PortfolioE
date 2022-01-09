// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "PortfolioE.h"
#include "Utils/PoolingActor.h"
#include "FloatingDamageText.generated.h"

/**
 * 
 */
UCLASS()
class PORTFOLIOE_API AFloatingDamageText : public APoolingActor
{
	GENERATED_BODY()

public:
	virtual void PostInitializeComponents() override;
	virtual void InActive() override;

	void ShowDamage(float Damage);

private:
	UPROPERTY()
	class UWidgetAnimation* FloatingDamageAnim;

	UPROPERTY()
	class UUserWidget* UserWidget;

	UPROPERTY()
	class UTextBlock* FloatingDamageText;
	
};
