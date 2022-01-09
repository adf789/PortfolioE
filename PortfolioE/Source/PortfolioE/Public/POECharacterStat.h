// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "PortfolioE.h"
#include "Components/ActorComponent.h"
#include "POECharacterStat.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnChangeStatusDelegate);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PORTFOLIOE_API UPOECharacterStat : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPOECharacterStat();

	virtual void InitializeComponent() override;

	void InitValue(float MaxHp, float Atk, float Def);
	void SetHPValue(float CurHP);
	float GetHpRate();
	FOnChangeStatusDelegate OnDyingAction;
	FOnChangeStatusDelegate OnChangeHPAction;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Status, Meta = (AllowPrivateAccess = true))
	float CurrentHPValue;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Status, Meta = (AllowPrivateAccess = true))
	float MaxHPValue;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Status, Meta = (AllowPrivateAccess = true))
	float AttackValue;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Status, Meta = (AllowPrivateAccess = true))
	float DefenseValue;
	
};
