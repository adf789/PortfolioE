// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "PortfolioE.h"
#include "GameFramework/Character.h"
#include "POEMonster.generated.h"

DECLARE_DELEGATE(FOnAttackEndDelegate);

UCLASS()
class PORTFOLIOE_API APOEMonster : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APOEMonster();

	FOnAttackEndDelegate OnAttackEndDelegate;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual float GetAttackRange();
	virtual void Attack();
	virtual void OnMontageEnded();

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = State, Meta = (AllowPrivateAccess = true))
	bool IsAttacking;
};
