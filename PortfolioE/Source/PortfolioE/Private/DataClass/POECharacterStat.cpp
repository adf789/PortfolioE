// Description
// 캐릭터의 능력치 데이터를 담고있는 클래스

#include "POECharacterStat.h"


// Sets default values for this component's properties
UPOECharacterStat::UPOECharacterStat()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


void UPOECharacterStat::InitializeComponent()
{
	Super::InitializeComponent();
	CurrentHPValue = MaxHPValue;
}

void UPOECharacterStat::SetHPValue(float CurHP)
{
	if (CurrentHPValue == CurHP) return;
	CurrentHPValue = CurHP;

	if (CurrentHPValue > MaxHPValue) CurrentHPValue = MaxHPValue;

	if (OnChangeHPAction.IsBound()) OnChangeHPAction.Broadcast();

	if (CurrentHPValue < KINDA_SMALL_NUMBER) {
		if (OnDyingAction.IsBound()) OnDyingAction.Broadcast();
		CurrentHPValue = .0f;
	}
}

void UPOECharacterStat::InitHPVale(float MaxHp)
{
	this->MaxHPValue = MaxHp;
	this->CurrentHPValue = MaxHp;
}

void UPOECharacterStat::InitAttackValue(float Atk)
{
	this->AttackValue = Atk;
}

void UPOECharacterStat::InitMoveSpeedValue(float Speed)
{
	this->MoveSpeedValue = Speed;
}

void UPOECharacterStat::InitMPValue(float MaxMp)
{
	this->MaxMPValue = MaxMp;
	this->CurrentMPValue = MaxMp;
}

void UPOECharacterStat::SetMPValue(float CurMP)
{
	CurrentMPValue = CurMP;
	if (CurrentMPValue > MaxMPValue) this->CurrentMPValue = MaxMPValue;

	if (CurrentMPValue > CurMP) CurrentMPValue = CurMP;
	else if (CurrentMPValue < KINDA_SMALL_NUMBER) CurrentMPValue = .0f;

	if (OnChangeMPAction.IsBound()) OnChangeMPAction.Broadcast();
}

float UPOECharacterStat::GetHPRate()
{
	if (MaxHPValue == .0f) return 1.0f;
	else if (CurrentHPValue < .0f) return .0f;
	else return CurrentHPValue / MaxHPValue;
}

float UPOECharacterStat::GetMPRate()
{
	if (MaxMPValue == .0f) return 1.0f;
	else if (CurrentMPValue < .0f) return .0f;
	else return CurrentMPValue / MaxMPValue;
}

