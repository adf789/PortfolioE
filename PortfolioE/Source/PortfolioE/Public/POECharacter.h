// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "PortfolioE.h"
#include "GameFramework/Character.h"
#include "POECharacter.generated.h"

UCLASS()
class PORTFOLIOE_API APOECharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APOECharacter();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void PossessedBy(AController* NewController) override;
	virtual void PostInitializeComponents() override;

	void MeleeAttack(FVector Direction);

	UFUNCTION()
	void OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted);

	UFUNCTION(BlueprintCallable)
	void SetAttackType();
	void CheckAttackRange();
	void CheckAttackCombo();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	enum class EControlType {
		Player,
		Monster
	};

	void SetControlMode(EControlType ControlType);
	void ActiveSkill();

private:
	void SetDestination();
	void ClickTarget();


public:
	UPROPERTY(VisibleAnywhere, Category = Camera)
	USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere, Category = Camera)
	UCameraComponent* Camera;


private:
	UPROPERTY()
	class APOEPlayerController* POEPlayerController;

	bool CheckMouseDrag;
	bool CheckNPC;

	UPROPERTY()
	class UPOECharacterAnimInstance* CharacterAnim;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	bool IsAttacking;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	bool IsComboInput;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	int32 CurrentCombo;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	int32 MaxCombo;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	float AttackRange;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	bool IsRangeAttack;
};
