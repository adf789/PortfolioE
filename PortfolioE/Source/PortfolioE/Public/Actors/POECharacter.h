// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "PortfolioE.h"
#include "POECharacter_Base.h"
#include "UIScreenInteraction.h"
#include "POECharacter.generated.h"

UCLASS()
class PORTFOLIOE_API APOECharacter : public APOECharacter_Base
{
	GENERATED_BODY()

#pragma region Function
public:
	// Sets default values for this character's properties
	APOECharacter();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void PossessedBy(AController* NewController) override;
	virtual void PostInitializeComponents() override;
	virtual void CheckMeleeAttackCollision() override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	void MeleeAttack();

	UFUNCTION()
	virtual void OnAnimMontageEnded(UAnimMontage* Montage, bool bInterrupted) override;

	UFUNCTION(BlueprintCallable)
		void SetAttackType();
	void CheckAttackCombo();
	bool IsPlayingMontionAnything();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void LoadInventoryData();

	void ActiveAction();
	void Dash();
	bool GetCurDestination(FHitResult& HitResult);
	void SetDestination();
	void ClickTarget();
	void CastingSpell(FVector Location);
	void ChangeActive(int index);

private:
	UFUNCTION()
	void CalculateCoolTime();
#pragma endregion

#pragma region Variables
public:
	UPROPERTY(VisibleAnywhere, Category = Camera)
	USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere, Category = Camera)
	UCameraComponent* Camera;

	UPROPERTY(VisibleAnywhere, Category = Collision)
	UCapsuleComponent* MeleeCollision;

	UPROPERTY(VisibleAnywhere, Category = Minimap)
	USpringArmComponent* SpringArmForCapture;

	UPROPERTY(VisibleAnywhere, Category = Camera)
	class USceneCaptureComponent2D* CaptureCamera;

	UPROPERTY(EditAnywhere, Category = Minimap)
	class UPaperSpriteComponent* ArrowSprite;

	UPROPERTY(VisibleInstanceOnly, Category = Inventory)
	class UMyInventoryComponent* Inventory;

	UPROPERTY(VisibleInstanceOnly, Category = UI)
	UUIScreenInteraction* UIScreens;

private:
	UPROPERTY()
		class APOEPlayerController* POEPlayerController;

	bool CheckMouseDrag;
	bool CheckNPC;

	UPROPERTY()
		class UPOECharacterAnimInstance* CharacterAnim;

	UPROPERTY()
		class UParticleSystem* LavaEffect;

	UPROPERTY()
		class UParticleSystem* LightningEffect;

	UPROPERTY()
		class UParticleSystem* SelectedEffect;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = State, Meta = (AllowPrivateAccess = true))
		bool IsRangeAttack;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = State, Meta = (AllowPrivateAccess = true))
		bool IsSprinting;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = State, Meta = (AllowPrivateAccess = true))
		bool IsCasting;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		bool IsComboInput;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		int32 CurrentCombo;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		int32 MaxCombo;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		bool AlreadyAttackColiision;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = CoolTime, Meta = (AllowPrivateAccess = true))
		float DashCoolTime;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Material, Meta = (AllowPrivateAccess = true))
		class UMaterialInstanceDynamic* SmearMaterialInstance;

	FTimerHandle CoolTimeHandle;
#pragma endregion
};
