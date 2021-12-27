// Fill out your copyright notice in the Description page of Project Settings.

#include "POECharacterAnimInstance.h"

UPOECharacterAnimInstance::UPOECharacterAnimInstance() {
	PawnSpeed = .0f;
	IsDead = false;

	static ConstructorHelpers::FObjectFinder<UAnimMontage>
		MELEE_ATTACK_MONTAGE(TEXT("/Game/POE/Animations/SevarogMeleeAttack_Montage.SevarogMeleeAttack_Montage"));
	if (MELEE_ATTACK_MONTAGE.Succeeded()) {
		MeleeAttackMontage = MELEE_ATTACK_MONTAGE.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage>
		DASH_MONTAGE(TEXT("/Game/POE/Animations/SevarogDash_Montage.SevarogDash_Montage"));
	if (DASH_MONTAGE.Succeeded()) {
		DashMontage = DASH_MONTAGE.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage>
		CAST_MONTAGE(TEXT("/Game/POE/Animations/SevarogMagicAttack_Montage.SevarogMagicAttack_Montage"));
	if (CAST_MONTAGE.Succeeded()) {
		CastMontage = CAST_MONTAGE.Object;
	}
}

void UPOECharacterAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	auto Pawn = TryGetPawnOwner();
	if (::IsValid(Pawn)) {
		if (!IsDead) {
			PawnSpeed = Pawn->GetVelocity().Size();
		}
	}
}

void UPOECharacterAnimInstance::PlayMeleeAttack()
{
	if (!IsDead) {
		Montage_Play(MeleeAttackMontage, 1.0f);
	}
}

void UPOECharacterAnimInstance::JumpToAttackMeleeCombo(int32 NewSection)
{
	if (!IsDead && Montage_IsPlaying(MeleeAttackMontage)) {
		Montage_JumpToSection(GetMeleeAttackSectionName(NewSection), MeleeAttackMontage);
	}
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

void UPOECharacterAnimInstance::AnimNotify_AttackHit()
{
	OnAttackCollision.Broadcast();
}

void UPOECharacterAnimInstance::AnimNotify_ContinueCombo()
{
	OnNextComboCheck.Broadcast();
}

void UPOECharacterAnimInstance::AnimNotify_SwayMeleeAttack()
{
	//OnAttackSway.Execute();
	ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwningActor());
	CHECKRETURN(!::IsValid(OwnerCharacter));
	if (OwnerCharacter != nullptr) {
		OwnerCharacter->GetCharacterMovement()->AddImpulse(OwnerCharacter->GetActorForwardVector() * 500.0f * OwnerCharacter->GetMesh()->GetMass(), true);
	}
}

FName UPOECharacterAnimInstance::GetMeleeAttackSectionName(int32 Section)
{
	if (FMath::IsWithinInclusive<int32>(Section, 1, 3)) {
		return FName(*FString::Printf(TEXT("Attack%d"), Section));
	}
	return NAME_None;
}
