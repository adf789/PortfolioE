// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "PortfolioE.h"
#include "Blueprint/UserWidget.h"
#include "POEStageStartWidget.generated.h"

#define MAX_STAGE 10

/**
 * 
 */
UCLASS()
class PORTFOLIOE_API UPOEStageStartWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
		void OnCancel();

	UFUNCTION(BlueprintCallable)
		void OnStageStart();

protected:
	virtual void NativeConstruct() override;
	UFUNCTION()
	void PrevStage();
	UFUNCTION()
	void NextStage();
	UFUNCTION()
	void SetStage();
	void SpawnNormalMonster(UClass* MonsterClass, FVector Location);

protected:
	UPROPERTY()
	class UButton* PrevStageButton;

	UPROPERTY()
	class UButton* NextStageButton;

	UPROPERTY()
	class UTextBlock* CurrentStageNumber;

	UPROPERTY()
	class UPOEGameInstance* GameInstance;
};
