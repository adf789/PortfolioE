// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "POECharacter_Base.h"
#include "POEMonster_Base.generated.h"

/**
 * 
 */
UCLASS()
class PORTFOLIOE_API APOEMonster_Base : public APOECharacter_Base
{
	GENERATED_BODY()
	
public:
	APOEMonster_Base();

	virtual void PostInitializeComponents() override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	virtual void CheckMeleeAttackCollision() override;
	virtual float GetAIDetectRange() override;
	virtual void Die() override;
	virtual void Active();
	virtual void InActive();

	float GetAttackDelay();

protected:
	virtual void BeginPlay() override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = State, Meta = (AllowPrivateAccess = true))
	bool IsSpawned;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Status, Meta = (AllowPrivateAccess = true))
	float DelayAfterAttack;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Status, Meta = (AllowPrivateAccess = true))
	float AIDetectDistance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Attack, Meta = (AllowPrivateAccess = true))
	float AttackCollisionScale;

	UPROPERTY(EditAnywhere, Category = Minimap)
	class UPaperSpriteComponent* ArrowSprite;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Material, Meta = (AllowPrivateAccess = true))
	class UMaterialInstanceDynamic* VisibilityMaterialInstance;

	FTimerHandle InvisibilityTimerHandle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Material, Meta = (AllowPrivateAccess = true))
	float InvisibilityAmount;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = State)
	float RestTimer;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = State)
	float MonsterId;
};
