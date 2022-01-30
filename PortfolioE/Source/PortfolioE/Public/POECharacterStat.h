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

	void SetHPValue(float CurHP);
	void InitHPVale(float MaxHp);
	void InitAttackValue(float Atk);
	void InitDefenceValue(float Def);
	void InitMPValue(float MaxMp);
	void SetMPValue(float CurMP);
	float GetHPRate();
	float GetMPRate();

	FOnChangeStatusDelegate OnDyingAction;
	FOnChangeStatusDelegate OnChangeHPAction;
	FOnChangeStatusDelegate OnChangeMPAction;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Status, Meta = (AllowPrivateAccess = true))
	float CurrentHPValue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Status, Meta = (AllowPrivateAccess = true))
	float MaxHPValue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Status, Meta = (AllowPrivateAccess = true))
	float AttackValue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Status, Meta = (AllowPrivateAccess = true))
	float DefenseValue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Status, Meta = (AllowPrivateAccess = true))
	float CurrentMPValue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Status, Meta = (AllowPrivateAccess = true))
	float MaxMPValue;
	
};
