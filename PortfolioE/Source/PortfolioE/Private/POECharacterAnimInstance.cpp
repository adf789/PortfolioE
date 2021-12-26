// Fill out your copyright notice in the Description page of Project Settings.

#include "POECharacterAnimInstance.h"

UPOECharacterAnimInstance::UPOECharacterAnimInstance() {
	pawnSpeed = .0f;
	isDead = false;

	static ConstructorHelpers::FObjectFinder<UAnimMontage>
		MELEE_ATTACK_MONTAGE(TEXT("/Game/POE/Animations/SevarogMeleeAttack_Montage.SevarogMeleeAttack_Montage"));
	if (MELEE_ATTACK_MONTAGE.Succeeded()) {
		meleeAttackMontage = MELEE_ATTACK_MONTAGE.Object;
	}
}

void UPOECharacterAnimInstance::NativeUpdateAnimation(float deltaSeconds)
{
	auto Pawn = TryGetPawnOwner();
	if (::IsValid(Pawn)) {
		if (!isDead) {
			pawnSpeed = Pawn->GetVelocity().Size();
		}
	}
}

void UPOECharacterAnimInstance::PlayMeleeAttack()
{
	if (!isDead) {
		Montage_Play(meleeAttackMontage, 1.0f);
	}
}

void UPOECharacterAnimInstance::JumpToAttackMeleeCombo(int32 newSection)
{
	if (!isDead && Montage_IsPlaying(meleeAttackMontage)) {
		Montage_JumpToSection(GetMeleeAttackSectionName(newSection), meleeAttackMontage);
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
	ACharacter* ownerCharacter = Cast<ACharacter>(GetOwningActor());
	CHECKRETURN(!::IsValid(ownerCharacter));
	if (ownerCharacter != nullptr) {
		ownerCharacter->GetCharacterMovement()->AddImpulse(ownerCharacter->GetActorForwardVector() * 500.0f * ownerCharacter->GetMesh()->GetMass(), true);
	}
}

FName UPOECharacterAnimInstance::GetMeleeAttackSectionName(int32 section)
{
	if (FMath::IsWithinInclusive<int32>(section, 1, 3)) {
		return FName(*FString::Printf(TEXT("Attack%d"), section));
	}
	return NAME_None;
}
