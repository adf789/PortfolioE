// Fill out your copyright notice in the Description page of Project Settings.

#include "POECharacter_Base.h"
#include "AnimInstance_Base.h"


// Sets default values
APOECharacter_Base::APOECharacter_Base()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetCapsuleComponent()->SetCollisionProfileName(TEXT("POECharacter"));
	GetMesh()->SetCollisionProfileName(TEXT("POECharacter"));
}

// Called when the game starts or when spawned
void APOECharacter_Base::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APOECharacter_Base::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APOECharacter_Base::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	AnimInstance = Cast<UAnimInstance_Base>(GetMesh()->GetAnimInstance());
	CHECKRETURN(AnimInstance == nullptr);

	AnimInstance->OnMontageEnded.AddDynamic(this, &APOECharacter_Base::OnAnimMontageEnded);
}

void APOECharacter_Base::Attack()
{
	if (AnimInstance != nullptr) {
		IsAttacking = true;
		if (!DontMotion) {
			AnimInstance->PlayAttack();
		}

		if (!ContinousMotion) DontMotion = false;
	}
}

float APOECharacter_Base::GetAttackRange()
{
	return AttackRange;
}

void APOECharacter_Base::CheckMeleeAttackCollision()
{
}

float APOECharacter_Base::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser)
{
	float totalDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	if (AnimInstance != nullptr) {
		IsHitting = true;

		FVector AttackDirection = GetActorLocation() - DamageCauser->GetActorLocation();
		if (!DontMotion) {
			AnimInstance->PlayHit(AttackDirection);
		}

		if (!ContinousMotion) DontMotion = false;
	}

	return totalDamage;
}

void APOECharacter_Base::OnAnimMontageEnded(UAnimMontage * Montage, bool bInterrupted)
{
	if (IsAttacking) {
		if(OnAttackEndDelegate.IsBound()) OnAttackEndDelegate.Broadcast();
		IsAttacking = false;
	}
	if (IsHitting) {
		if(OnHitEndDelegate.IsBound()) OnHitEndDelegate.Broadcast();
		IsHitting = false;
	}
}

void APOECharacter_Base::DontMotionAtOnce()
{
	ContinousMotion = false;
	DontMotion = true;
}

void APOECharacter_Base::SetDontMotion(bool DontMontion)
{
	ContinousMotion = true;
	this->DontMotion = DontMotion;
}

