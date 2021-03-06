// Fill out your copyright notice in the Description page of Project Settings.

#include "POECharacterAnimInstance.h"

UPOECharacterAnimInstance::UPOECharacterAnimInstance() {
	static ConstructorHelpers::FObjectFinder<UAnimMontage>
		MELEE_ATTACK_MONTAGE(TEXT("/Game/POE/Animations/Sevarog/SevarogMeleeAttack_Montage.SevarogMeleeAttack_Montage"));
	if (MELEE_ATTACK_MONTAGE.Succeeded()) {
		MeleeAttackMontage = MELEE_ATTACK_MONTAGE.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage>
		DASH_MONTAGE(TEXT("/Game/POE/Animations/Sevarog/SevarogDash_Montage.SevarogDash_Montage"));
	if (DASH_MONTAGE.Succeeded()) {
		DashMontage = DASH_MONTAGE.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage>
		CAST_MONTAGE(TEXT("/Game/POE/Animations/Sevarog/SevarogMagicAttack_Montage.SevarogMagicAttack_Montage"));
	if (CAST_MONTAGE.Succeeded()) {
		CastMontage = CAST_MONTAGE.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage>
		HIT_MONTAGE(TEXT("/Game/POE/Animations/Sevarog/SevarogHit_Montage.SevarogHit_Montage"));
	if (HIT_MONTAGE.Succeeded()) {
		HitMontage = HIT_MONTAGE.Object;
	}

	MaxAttackComboIndex = 3;
	CurAttackComboIndex = 1;
}

void UPOECharacterAnimInstance::PlayAttackCombo(int32 Section)
{
	CurAttackComboIndex = Section;
	Super::PlayAttack();
}

void UPOECharacterAnimInstance::PlayDash()
{
	if (!IsDead && !Montage_IsPlaying(DashMontage)) {
		Montage_Play(DashMontage);
	}
}

void UPOECharacterAnimInstance::PlayCastMagic()
{
	if (!IsDead && !Montage_IsPlaying(CastMontage)) {
		Montage_Play(CastMontage);
	}
}

void UPOECharacterAnimInstance::PlayAttackMotion()
{
	Super::PlayAttackMotion();
	if (!IsDead) {
		if (Montage_IsPlaying(MeleeAttackMontage)) {
			Montage_JumpToSection(GetMeleeAttackSectionName(CurAttackComboIndex), MeleeAttackMontage);
		}
		else Montage_Play(MeleeAttackMontage, 1.0f);
	}
}

void UPOECharacterAnimInstance::PlayHitMotion(FVector Direction)
{
	Super::PlayHitMotion(Direction);
	if (IsDead) return;

	if (Montage_IsPlaying(HitMontage)) return;
	FVector DirNormalized = Direction.GetSafeNormal();
	float DotProduct = FVector::DotProduct(DirNormalized, GetOwningActor()->GetActorForwardVector().GetSafeNormal());

	Montage_Play(HitMontage);
	if (DotProduct <= 1 && DotProduct > 0.5) {
		Montage_JumpToSection(TEXT("Hit_Back"), HitMontage);
	}
}

void UPOECharacterAnimInstance::AnimNotify_ContinueCombo()
{
	if(OnNextComboCheck.IsBound()) OnNextComboCheck.Broadcast();
}

void UPOECharacterAnimInstance::AnimNotify_SwayMeleeAttack()
{
	//OnAttackSway.Execute();
	ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwningActor());
	CHECKRETURN(!::IsValid(OwnerCharacter));
	if (OwnerCharacter != nullptr) {
		OwnerCharacter->LaunchCharacter(OwnerCharacter->GetActorForwardVector() * 500.0f, false, false);
	}
}

void UPOECharacterAnimInstance::AnimNotify_MeleeCollision()
{
}

FName UPOECharacterAnimInstance::GetMeleeAttackSectionName(int32 Section)
{
	if (FMath::IsWithinInclusive<int32>(Section, 1, MaxAttackComboIndex)) {
		return FName(*FString::Printf(TEXT("Attack%d"), Section));
	}
	return NAME_None;
}
