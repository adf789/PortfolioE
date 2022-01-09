// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "PortfolioE.h"
#include "Blueprint/UserWidget.h"
#include "POECharacterHPWidget.generated.h"

/**
 * 
 */
UCLASS()
class PORTFOLIOE_API UPOECharacterHPWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void BindCharacterStat(class UPOECharacterStat* CharacterStatComponent);

	UFUNCTION()
		void UpdateHPWidget();
	
protected:
	virtual void NativeConstruct() override;

private:
	UPROPERTY()
		class UProgressBar* HpBar;


	UPROPERTY()
	class UPOECharacterStat* CharacterStat;
};
