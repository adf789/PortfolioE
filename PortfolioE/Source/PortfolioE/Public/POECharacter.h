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

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	enum class EControlType {
		Player,
		NPC,
		Monster
	};

	void SetControlMode(EControlType ControlType);
	void ActiveSkill();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void PossessedBy(AController* NewController) override;

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
	class APOEPlayerController* poePlayerController;

	bool CheckMouseDrag;
	bool CheckNPC;
};
