// Fill out your copyright notice in the Description page of Project Settings.

#include "POECharacterStat.h"


// Sets default values for this component's properties
UPOECharacterStat::UPOECharacterStat()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


void UPOECharacterStat::InitializeComponent()
{
	Super::InitializeComponent();
	CurrentHPValue = MaxHPValue;
}

void UPOECharacterStat::InitValue(float MaxHp, float Atk, float Def)
{
	this->MaxHPValue = MaxHp;
	this->CurrentHPValue = MaxHPValue;
	this->AttackValue = Atk;
	this->DefenseValue = Def;
}

void UPOECharacterStat::SetHPValue(float CurHP)
{
	if (CurrentHPValue == CurHP) return;
	CurrentHPValue = CurHP;

	if (OnChangeHPAction.IsBound()) OnChangeHPAction.Broadcast();

	if (CurrentHPValue < KINDA_SMALL_NUMBER) {
		if (OnDyingAction.IsBound()) OnDyingAction.Broadcast();
		CurrentHPValue = .0f;
	}
}

float UPOECharacterStat::GetHpRate()
{
	if (MaxHPValue == .0f) return 1.0f;
	else if (CurrentHPValue < .0f) return .0f;
	else return CurrentHPValue / MaxHPValue;
}

// Called when the game starts
void UPOECharacterStat::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UPOECharacterStat::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

