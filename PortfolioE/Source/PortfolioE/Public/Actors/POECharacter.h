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
	APOECharacter();

	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void PossessedBy(AController* NewController) override;
	virtual void PostInitializeComponents() override;
	virtual void CheckMeleeAttackCollision() override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	
	void MeleeAttack();
	void ApplyCharacterStatus();

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

	void PrepareMeleeAttack();
	void Dash();
	bool GetCurDestination(FHitResult& HitResult);
	void SetDestination();
	void ClickTarget();
	void CastingSpell(FVector Location);
	void PrepareCastingSpell();

private:
	void BindCoolTime();
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

	UPROPERTY(VisibleInstanceOnly, Category = UI)
	UUIScreenInteraction* UIScreens;

	UPROPERTY(VisibleInstanceOnly, Category = Effect)
	class UParticleSystemComponent* BuffEffect;

	UPROPERTY(VisibleInstanceOnly, Category = Inventory)
		class UMyInventoryComponent* Inventory;

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
	class UParticleSystem* IceEffect;

	UPROPERTY()
	class UParticleSystem* StrengthEffect;

	UPROPERTY()
	class UParticleSystem* DemonEffect;

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
	bool DashCoolTime;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = CoolTime, Meta = (AllowPrivateAccess = true))
	bool ActiveCoolTime;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Material, Meta = (AllowPrivateAccess = true))
	class UMaterialInstanceDynamic* SmearMaterialInstance;

	FTimerHandle CoolTimeHandle;

	class UPOEGameInstance* GameInstance;
#pragma endregion
};
