// Fill out your copyright notice in the Description page of Project Settings.

#include "POECharacter_Base.h"
#include "AnimInstance_Base.h"
#include "POEPlayerController.h"
#include "POEDamageWidget.h"
#include "Components/TextBlock.h"
#include "FloatingDamageText.h"
#include "POEGameInstance.h"
#include "ActorObjectPool.h"
#include "POECharacterHPWidget.h"
#include "Components/WidgetComponent.h"


// Sets default values
APOECharacter_Base::APOECharacter_Base()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StatusWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("StatusWidget"));
	CharacterStatus = CreateDefaultSubobject<UPOECharacterStat>(TEXT("CharacterStatus"));

	GetCapsuleComponent()->SetCollisionProfileName(TEXT("POECharacter"));
	GetMesh()->SetCollisionProfileName(TEXT("POECharacter"));
	StatusWidget->SetupAttachment(GetMesh());
	
	static ConstructorHelpers::FClassFinder<AFloatingDamageText>
		FLOATING_DAMAGE_TEXT_C(TEXT("/Game/POE/Blueprints/UI/BP_FloatingDamageText.BP_FloatingDamageText_c"));
	if (FLOATING_DAMAGE_TEXT_C.Succeeded()) {
		FloatingDamageClass2 = FLOATING_DAMAGE_TEXT_C.Class;
	}

	StatusWidget->SetWorldLocation(FVector(.0f, .0f, 280.0f));
	StatusWidget->SetWidgetSpace(EWidgetSpace::Screen);
	static ConstructorHelpers::FClassFinder<UUserWidget>
		UI_STATUS_VIEW_C(TEXT("/Game/POE/UIWidget/UI_ActorStatusView.UI_ActorStatusView_c"));
	if (UI_STATUS_VIEW_C.Succeeded()) {
		StatusWidget->SetWidgetClass(UI_STATUS_VIEW_C.Class);
		StatusWidget->SetDrawSize(FVector2D(200.0f, 60.0f));
	}
	StatusWidget->SetHiddenInGame(true);
}

// Called when the game starts or when spawned
void APOECharacter_Base::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void APOECharacter_Base::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APOECharacter_Base::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	AnimInstance = Cast<UAnimInstance_Base>(GetMesh()->GetAnimInstance());
	CHECKRETURN(AnimInstance == nullptr);

	AnimInstance->OnMontageEnded.AddDynamic(this, &APOECharacter_Base::OnAnimMontageEnded);

	StatusWidget->InitWidget();
	UPOECharacterHPWidget* TempWidget = Cast<UPOECharacterHPWidget>(StatusWidget->GetUserWidgetObject());
	if (TempWidget != nullptr) {
		TempWidget->BindCharacterStat(CharacterStatus);
	}
}

void APOECharacter_Base::Attack()
{
	if (AnimInstance != nullptr) {
		IsAttacking = true;
		if (!DontMotion) {
			AnimInstance->PlayAttack();
		}

		if (!ContinousMotion) DontMotion = false;
	}
}

float APOECharacter_Base::GetAttackRange()
{
	return AttackRange;
}

void APOECharacter_Base::CheckMeleeAttackCollision()
{
}

float APOECharacter_Base::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser)
{
	float totalDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	if (AnimInstance != nullptr) {
		IsHitting = true;

		FVector AttackDirection = GetActorLocation() - DamageCauser->GetActorLocation();
		if (!DontMotion) {
			AnimInstance->PlayHit(AttackDirection);
		}

		if (!ContinousMotion) DontMotion = false;
	}
	APOEPlayerController* PlayerController = Cast<APOEPlayerController>(GetWorld()->GetFirstPlayerController());
	if (FloatingDamageClass2 != nullptr && PlayerController != nullptr) {
		UPOEGameInstance* POEGameInstance = Cast<UPOEGameInstance>(GetGameInstance());

		AFloatingDamageText* TempDamageText = nullptr;
		if (POEGameInstance != nullptr) {
			TempDamageText = Cast< AFloatingDamageText>(POEGameInstance->DamageTextPooling->GetUnUseObject());
		}
		
		if (TempDamageText == nullptr) {
			TempDamageText = GetWorld()->SpawnActor<AFloatingDamageText>(FloatingDamageClass2, GetActorLocation(), FRotator::ZeroRotator);
		}
		else {
			TempDamageText->SetActorLocation(GetActorLocation());
		}

		TempDamageText->ShowDamage(DamageAmount);
		CharacterStatus->SetHPValue(CharacterStatus->CurrentHPValue - DamageAmount);
		StatusWidget->SetHiddenInGame(false);
	}
	return totalDamage;
}

void APOECharacter_Base::OnAnimMontageEnded(UAnimMontage * Montage, bool bInterrupted)
{
	if (IsAttacking) {
		if(OnAttackEndDelegate.IsBound()) OnAttackEndDelegate.Broadcast();
		IsAttacking = false;
	}
	if (IsHitting) {
		if(OnHitEndDelegate.IsBound()) OnHitEndDelegate.Broadcast();
		IsHitting = false;
	}
}

void APOECharacter_Base::DontMotionAtOnce()
{
	ContinousMotion = false;
	DontMotion = true;
}

void APOECharacter_Base::SetDontMotion(bool DontMontion)
{
	ContinousMotion = true;
	this->DontMotion = DontMotion;
}

float APOECharacter_Base::GetHP()
{
	return CharacterStatus->CurrentHPValue;
}

