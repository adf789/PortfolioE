// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "PortfolioE.h"
#include "BaseClass/POEMonster_Base.h"
#include "POEMonster_Lottery.generated.h"

/**
 * 
 */
UCLASS()
class PORTFOLIOE_API APOEMonster_Lottery : public APOEMonster_Base
{
	GENERATED_BODY()
	
public:
	virtual void PossessedBy(AController* NewController) override;
	void BindFunctionForRunAI();
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

private:
	class APOEMonsterAIController* AIController;
	FTimerHandle SpeedUpTimerHandle;
};
