// Fill out your copyright notice in the Description page of Project Settings.

#include "POEMonster_Base.h"
#include "Animinstance_Base.h"
#include "POEMonsterAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "DrawDebugHelpers.h"
#include "PaperSpriteComponent.h"
#include "PaperSprite.h"
#include "POEGameInstance.h"
#include "ActorObjectPool.h"

APOEMonster_Base::APOEMonster_Base() {
	AIControllerClass = APOEMonsterAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	ArrowSprite = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("ArrowSprite"));

	ArrowSprite->SetupAttachment(GetCapsuleComponent());
	ArrowSprite->SetWorldScale3D(FVector(.1f, .1f, .1f));
	ArrowSprite->SetWorldLocationAndRotation(FVector(.0f, .0f, 4000.0f), FRotator(.0f, .0f, 90.0f));
	ArrowSprite->SetOwnerNoSee(true);

	static ConstructorHelpers::FObjectFinder<UPaperSprite>
		SPRITE_ARROW(TEXT("/Game/POE/Effects/Textures/T_TransparentArrow_Sprite.T_TransparentArrow_Sprite"));
	if (SPRITE_ARROW.Succeeded()) {
		ArrowSprite->SetSprite(SPRITE_ARROW.Object);
		ArrowSprite->SetSpriteColor(FLinearColor::Red);
	}

	IsSpawned = false;
	AIControllerClass = APOEMonsterAIController::StaticClass();
}

void APOEMonster_Base::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	CHECKRETURN(CharacterStatus == nullptr);
	GetCharacterMovement()->MaxWalkSpeed = CharacterStatus->MoveSpeedValue;

	APOEMonsterAIController* AIController = Cast<APOEMonsterAIController>(GetController());
	CHECKRETURN(AIController == nullptr);
	AnimInstance->OnSpawnEnd.AddUObject(AIController, &APOEMonsterAIController::RunAI);

	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bUseControllerDesiredRotation = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(.0f, 480.0f, .0f);
}

float APOEMonster_Base::TakeDamage(float DamageAmount, FDamageEvent const & DamageEvent, AController * EventInstigator, AActor * DamageCauser)
{
	float TempDamage = DamageAmount;
	if (TempDamage < KINDA_SMALL_NUMBER) TempDamage = .0f;

	Super::TakeDamage(TempDamage, DamageEvent, EventInstigator, DamageCauser);
	return TempDamage;
}

void APOEMonster_Base::CheckMeleeAttackCollision()
{
	Super::CheckMeleeAttackCollision();

	FVector TraceDirection = GetActorForwardVector() * AttackRange;
	FVector CenterVector = GetActorLocation() + TraceDirection * .5f;
	FQuat TraceQuat = FRotationMatrix::MakeFromZ(TraceDirection).ToQuat();
	FCollisionQueryParams Params(NAME_None, false, this);
	TArray<FOverlapResult> OverlapResults;
	bool bResult = GetWorld()->OverlapMultiByChannel(
		OverlapResults,
		CenterVector,
		TraceQuat,
		ECollisionChannel::ECC_GameTraceChannel2,
		FCollisionShape::MakeCapsule(AttackCollisionScale * 1.5f, AttackRange * .5f),
		Params
	);

	if (bResult) {
		for (FOverlapResult OverlapResult : OverlapResults) {
			APOECharacter_Base* DetectCharacter = Cast<APOECharacter_Base>(OverlapResult.GetActor());
			if (DetectCharacter == nullptr) continue;

			AController* PlayerController = DetectCharacter->GetInstigatorController();
			if (PlayerController->IsPlayerController()) {
				FDamageEvent DamageEvent;
				DetectCharacter->TakeDamage(CharacterStatus->AttackValue, DamageEvent, PlayerController, this);
#if ENABLE_DRAW_DEBUG
				DrawDebugCapsule(GetWorld(), CenterVector, AttackRange * 0.5f, AttackCollisionScale * 1.5f, TraceQuat, FColor::Green, false, .1f);
#endif
				return;
			}
		}
	}
	else {
#if ENABLE_DRAW_DEBUG
		DrawDebugCapsule(GetWorld(), CenterVector, AttackRange * 0.5f, AttackCollisionScale * 1.5f, TraceQuat, FColor::Red, false, .1f);
#endif
	}
}

float APOEMonster_Base::GetAIDetectRange()
{
	return AIDetectDistance;
}
void APOEMonster_Base::Die()
{
	Super::Die();
	APOEMonsterAIController* MonsterAIController = Cast<APOEMonsterAIController>(GetController());
	if (MonsterAIController != nullptr) {
		MonsterAIController->StopAI();
	}

	if (VisibilityMaterialInstance == nullptr) {
		GetWorld()->GetTimerManager().SetTimer(InvisibilityTimerHandle, FTimerDelegate::CreateLambda([this]() {
			this->SetActorHiddenInGame(true);
			}), 3.0f, false);
	}
	else {
		GetWorld()->GetTimerManager().SetTimer(InvisibilityTimerHandle, FTimerDelegate::CreateLambda([this]() {
			if (InvisibilityAmount >= 4.0f) {
				InActive();
				GetWorld()->GetTimerManager().ClearTimer(this->InvisibilityTimerHandle);
				return;
			}

			InvisibilityAmount += 0.15f;
			VisibilityMaterialInstance->SetScalarParameterValue(TEXT("Amount"), InvisibilityAmount);
			}), 0.1f, true);
	}
}
void APOEMonster_Base::Active()
{
	SetActorHiddenInGame(false);
}
void APOEMonster_Base::InActive()
{
	SetActorHiddenInGame(true);

	UPOEGameInstance* POEGameInstance = Cast<UPOEGameInstance>(GetGameInstance());
	CHECKRETURN(POEGameInstance == nullptr);
	POEGameInstance->MonsterPooling->AddMonster(this);
}
float APOEMonster_Base::GetAttackDelay()
{
	return DelayAfterAttack;
}
void APOEMonster_Base::BeginPlay()
{
	Super::BeginPlay();

	InvisibilityAmount = 0;
}
