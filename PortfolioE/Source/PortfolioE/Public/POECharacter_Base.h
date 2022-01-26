// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "PortfolioE.h"
#include "GameFramework/Character.h"
#include "POECharacterStat.h"
#include "POECharacter_Base.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnAttackEndDelegate);
DECLARE_MULTICAST_DELEGATE(FOnHitEndDelegate);

UCLASS()
class PORTFOLIOE_API APOECharacter_Base : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APOECharacter_Base();

	FOnAttackEndDelegate OnAttackEndDelegate;
	FOnHitEndDelegate OnHitEndDelegate;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void PostInitializeComponents() override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	virtual void Attack();
	virtual void Die();
	virtual float GetAttackDistance();
	virtual void CheckMeleeAttackCollision();
	virtual float GetAIDetectRange();

	UFUNCTION()
	virtual void OnAnimMontageEnded(UAnimMontage* Montage, bool bInterrupted);
	void DontMotionAtOnce();
	void SetDontMotion(bool IsDontMontion);

	UFUNCTION(BlueprintCallable)
	virtual float GetHP();

	virtual ECharacterBehaviorState GetState() const;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Status)
	float AttackRange;

	UPROPERTY()
	class UAnimInstance_Base* AnimInstance;

	UPROPERTY()
	class UWidgetComponent* StatusWidget;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = CombatValue, Meta = (AllowPrivateAccess = true))
	UPOECharacterStat* CharacterStatus;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = State, Meta = (AllowPrivateAccess = true))
	bool IsAttacking;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = State, Meta = (AllowPrivateAccess = true))
	bool IsHitting;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = State, Meta = (AllowPrivateAccess = true))
	bool IsDead;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = State, Meta = (AllowPrivateAccess = true))
	bool DontMotion;

	UPROPERTY(VisibleAnywhere, Category = UI)
	class TSubclassOf<class AFloatingDamageText> FloatingDamageClass2;

	bool ContinousMotion;

	ECharacterBehaviorState CharacterState;
	
};
