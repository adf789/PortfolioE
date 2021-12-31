// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "POEMonster.h"
#include "POEGrux_Boss.generated.h"

/**
 * 
 */
UCLASS()
class PORTFOLIOE_API APOEGrux_Boss : public APOEMonster
{
	GENERATED_BODY()
	
public:
	// Sets default values for this character's properties
	APOEGrux_Boss();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void PostInitializeComponents() override;
	virtual float GetAttackRange() override;

private:
	UPROPERTY()
	class UPOEGruxAnimInstance* GruxAnim;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = State, Meta = (AllowPrivateAccess = true))
	bool IsSpawned;

	UPROPERTY()
	class APOEMonsterAIController* AIController;
};
