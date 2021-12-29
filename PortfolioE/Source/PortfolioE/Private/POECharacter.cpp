// Fill out your copyright notice in the Description page of Project Settings.

#include "POECharacter.h"
#include "Components/InputComponent.h"
#include "POEPlayerController.h"
#include "POENpcCharacter.h"
#include "POECharacterAnimInstance.h"
#include "DrawDebugHelpers.h"
#include "EffectDamageActor.h"
#include "ActorObjectPool.h"
#include "Kismet/GameplayStatics.h"
#include "Materials/MaterialInstanceConstant.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "POEGameInstance.h"


// Sets default values
APOECharacter::APOECharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SPRINGARM"));
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("CAMERA"));

	SpringArm->SetupAttachment(GetCapsuleComponent());
	Camera->SetupAttachment(SpringArm);

	static ConstructorHelpers::FObjectFinder<USkeletalMesh>
		SK_SEVAROG(TEXT("/Game/ParagonSevarog/Characters/Heroes/Sevarog/Meshes/Sevarog.Sevarog"));
	if (SK_SEVAROG.Succeeded()) {
		GetMesh()->SetSkeletalMesh(SK_SEVAROG.Object);
		GetMesh()->SetWorldRotation(FRotator(.0f, -120.0f, .0f));
	}

	static ConstructorHelpers::FObjectFinder<UMaterialInstanceConstant>
		MT_SMEAR(TEXT("/Game/ParagonSevarog/Characters/Heroes/Sevarog/Materials/M_Sevarog_Torso_2_Inst.M_Sevarog_Torso_2_Inst"));
	if (MT_SMEAR.Succeeded()) {
		SmearMaterialInstance = GetMesh()->CreateDynamicMaterialInstance(0, MT_SMEAR.Object);
		
		//GetMesh()->OverrideMaterials[0].
	}

	static ConstructorHelpers::FClassFinder<UAnimInstance>
		SAVAROG_ANIM(TEXT("/Game/POE/Animations/SevarogAnim.SevarogAnim_C"));
	if (SAVAROG_ANIM.Succeeded()) {
		GetMesh()->SetAnimInstanceClass(SAVAROG_ANIM.Class);
	}

	static ConstructorHelpers::FObjectFinder<UParticleSystem>
		PS_LAVA(TEXT("/Game/InfinityBladeEffects/Effects/FX_Ambient/Fire/P_Lava_Splashes.P_Lava_Splashes"));
	if (PS_LAVA.Succeeded()) {
		LavaEffect = PS_LAVA.Object;
	}

	static ConstructorHelpers::FObjectFinder<UParticleSystem>
		PS_LIGHTNING(TEXT("/Game/InfinityBladeEffects/Effects/FX_Monsters/FX_Monster_Genno/P_Genno_Overhead_Imp_01.P_Genno_Overhead_Imp_01"));
	if (PS_LIGHTNING.Succeeded()) {
		LightningEffect = PS_LIGHTNING.Object;
	}

	GetCapsuleComponent()->SetCollisionProfileName(TEXT("POECharacter"));
}

void APOECharacter::MeleeAttack()
{
	if (!IsAttacking) {
		IsAttacking = true;
		CharacterAnim->PlayMeleeAttack();
	}
	else if(IsComboInput){
		IsComboInput = false;
		CurrentCombo = FMath::Clamp(CurrentCombo + 1, 2, MaxCombo);
		CharacterAnim->JumpToAttackMeleeCombo(CurrentCombo);
	}
}

void APOECharacter::OnAnimMontageEnded(UAnimMontage* Montage, bool bInterrupted) {
	IsAttacking = false;
	IsComboInput = false;
	if(IsSprinting) SmearMaterialInstance->SetScalarParameterValue(TEXT("Amount"), 0);
	IsSprinting = false;
	IsCasting = false;
	CurrentCombo = 1;
}

void APOECharacter::SetAttackType()
{
	MaxCombo = 3;
	AttackRange = 50.0f;
	IsRangeAttack = false;
}

void APOECharacter::CheckAttackRange()
{
}

void APOECharacter::CheckAttackCombo()
{
	IsComboInput = true;
}

bool APOECharacter::IsPlayingMontionAnything()
{
	return IsAttacking || IsCasting || IsSprinting;
}

// Called when the game starts or when spawned
void APOECharacter::BeginPlay()
{
	Super::BeginPlay();
	
	SetAttackType();

	GetWorld()->GetTimerManager().SetTimer(CoolTimeHandle, this, &APOECharacter::CalculateCoolTime, 1.0f, true);
}

void APOECharacter::ActiveAction()
{
	if (IsPlayingMontionAnything() && !IsComboInput) return;
	GetCharacterMovement()->StopActiveMovement();

	FHitResult hitResult;
	bool bResult = POEPlayerController->GetHitResultUnderCursorByChannel(UEngineTypes::ConvertToTraceType(ECollisionChannel::ECC_GameTraceChannel1), true, hitResult);

	if (!bResult) return;

	FVector Direction = hitResult.Location - GetActorLocation();
	Direction.Z = .0f;
	FRotator Rot = FRotationMatrix::MakeFromX(Direction).Rotator();

	if (!IsPlayingMontionAnything()) {
		SetActorRotation(Rot);
	}
	
	if(!IsRangeAttack) MeleeAttack();
	else CastingSpell(hitResult.Location);
}

void APOECharacter::Dash()
{
	if (DashCoolTime > .0f) {
		TEST_LOG("Cooltime now");
		return;
	}
	else if (IsPlayingMontionAnything() && !IsComboInput) return;

	FHitResult hitResult;
	bool bResult = POEPlayerController->GetHitResultUnderCursorByChannel(UEngineTypes::ConvertToTraceType(ECollisionChannel::ECC_GameTraceChannel1), true, hitResult);

	if (bResult) {
		DashCoolTime = 5.0f;
		IsSprinting = true;

		GetCharacterMovement()->StopActiveMovement();
		FVector Direction = hitResult.Location - GetActorLocation();
		Direction.Z = .0f;
		FRotator Rot = FRotationMatrix::MakeFromX(Direction).Rotator();

		SetActorRotation(Rot);

		CharacterAnim->PlayDash();
		GetCharacterMovement()->AddImpulse(GetActorForwardVector() * 4000.0f * GetMesh()->GetMass(), true);
	}
}

bool APOECharacter::GetCurDestination(FHitResult & HitResult)
{
	if (IsPlayingMontionAnything() || POEPlayerController == nullptr) return false;
	
	bool bResult = POEPlayerController->GetHitResultUnderCursorByChannel(UEngineTypes::ConvertToTraceType(ECollisionChannel::ECC_GameTraceChannel1), true, HitResult);

	return bResult;
}

// Called every frame
void APOECharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (CheckMouseDrag) SetDestination();

	if (IsSprinting) {
		SmearMaterialInstance->SetVectorParameterValue(TEXT("Loc"), GetActorForwardVector() * -1.0f);
		float CurVelocity = FMath::Clamp(GetVelocity().Size(), 0.0f, 15000.0f);
		SmearMaterialInstance->SetScalarParameterValue(TEXT("Amount"), CurVelocity * 0.0001f);
	}
}

// Called to bind functionality to input
void APOECharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	FInputActionBinding Pressed(TEXT("SetDestination"), EInputEvent::IE_Pressed);
	Pressed.ActionDelegate.GetDelegateForManualSet().BindLambda([this]() {
		if (POEPlayerController != nullptr && POEPlayerController->IsDetectedNPC()) {
			CheckNPC = true;
			return;
		}
		CheckMouseDrag = true;
		});
	FInputActionBinding Released(TEXT("SetDestination"), EInputEvent::IE_Released);
	Released.ActionDelegate.GetDelegateForManualSet().BindLambda([this]() {
		if (CheckNPC) ClickTarget();
		CheckMouseDrag = false;
		});

	PlayerInputComponent->AddActionBinding(Pressed);
	PlayerInputComponent->AddActionBinding(Released);
	PlayerInputComponent->BindAction(TEXT("ActiveAction"), EInputEvent::IE_Pressed, this, &APOECharacter::ActiveAction);
	PlayerInputComponent->BindAction(TEXT("Dash"), EInputEvent::IE_Pressed, this, &APOECharacter::Dash);
	DECLARE_DELEGATE_OneParam(FCustomDelegate, int);
	PlayerInputComponent->BindAction<FCustomDelegate, APOECharacter, int>(TEXT("ChangeActiveQ"), EInputEvent::IE_Pressed, this, &APOECharacter::ChangeActive, 0);
	PlayerInputComponent->BindAction<FCustomDelegate, APOECharacter, int>(TEXT("ChangeActiveW"), EInputEvent::IE_Pressed, this, &APOECharacter::ChangeActive, 1);
	PlayerInputComponent->BindAction<FCustomDelegate, APOECharacter, int>(TEXT("ChangeActiveE"), EInputEvent::IE_Pressed, this, &APOECharacter::ChangeActive, 2);
	PlayerInputComponent->BindAction<FCustomDelegate, APOECharacter, int>(TEXT("ChangeActiveR"), EInputEvent::IE_Pressed, this, &APOECharacter::ChangeActive, 3);
}

void APOECharacter::PossessedBy(AController * NewController)
{
	Super::PossessedBy(NewController);

	if (IsPlayerControlled()) {
		SpringArm->TargetArmLength = 1500.0f;
		SpringArm->SetRelativeRotation(FRotator(-45.0f, .0f, .0f));
		SpringArm->bUsePawnControlRotation = false;
		SpringArm->bInheritPitch = false;
		SpringArm->bInheritRoll = false;
		SpringArm->bInheritYaw = false;
		SpringArm->bDoCollisionTest = false;
		bUseControllerRotationYaw = false;
		bUseControllerRotationPitch = true;
		bUseControllerRotationRoll = true;
		GetCharacterMovement()->bOrientRotationToMovement = true;
		GetCharacterMovement()->bUseControllerDesiredRotation = false;
		GetCharacterMovement()->RotationRate = FRotator(.0f, 720.0f, .0f);
	}

	POEPlayerController = Cast<APOEPlayerController>(NewController);
	CHECKRETURN(POEPlayerController == nullptr);
}

void APOECharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	CharacterAnim = Cast<UPOECharacterAnimInstance>(GetMesh()->GetAnimInstance());
	CHECKRETURN(CharacterAnim == nullptr);

	CharacterAnim->OnMontageEnded.AddDynamic(this, &APOECharacter::OnAnimMontageEnded);
	CharacterAnim->OnAttackCollision.AddUObject(this, &APOECharacter::CheckAttackRange);
	CharacterAnim->OnNextComboCheck.AddUObject(this, &APOECharacter::CheckAttackCombo);
}

void APOECharacter::SetDestination()
{
	FHitResult hitResult;
	bool bResult = GetCurDestination(hitResult);

#if ENABLE_DRAW_DEBUG
	DrawDebugSphere(GetWorld(),
		hitResult.Location,
		5.0f,
		8,
		bResult ? FColor::Green : FColor::Red,
		false,
		.5f
	);
#endif

	if (bResult) {
		FVector Direction = hitResult.Location - GetActorLocation();
		Direction.Z = .0f;
		FRotator Rot = FRotationMatrix::MakeFromX(Direction).Rotator();

		SetActorRotation(Rot);
		UNavigationSystem::SimpleMoveToLocation(GetController(), hitResult.Location);
	}

	if (CheckNPC && !POEPlayerController->IsDetectedNPC()) {
		POEPlayerController->HideNpcMenuWidget();
		CheckNPC = false;
	}
}

void APOECharacter::ClickTarget()
{
	CHECKRETURN(POEPlayerController == nullptr);

	FHitResult hitResult;
	bool bResult = POEPlayerController->GetHitResultUnderCursor(ECollisionChannel::ECC_WorldDynamic, true, hitResult);
	if (bResult) {
		AActor* actor = hitResult.GetActor();
		if (actor == nullptr || !actor->Tags.Contains(TEXT("NPC"))) {
			return;
		}

		UNavigationSystem::SimpleMoveToLocation(GetController(), GetActorLocation());
		APOENpcCharacter* character = Cast<APOENpcCharacter>(actor);
		if (character != nullptr) {
			POEPlayerController->ShowNpcMenuWidget(character);
		}
	}
}

void APOECharacter::CastingSpell(FVector Location)
{
	if (IsCasting) return;
	IsCasting = true;
	CharacterAnim->PlayCastMagic();

	AEffectDamageActor* NewEffect = nullptr;
	bool bPooling = true;
	UPOEGameInstance* PoeInstance = Cast<UPOEGameInstance>(GetGameInstance());
	if (PoeInstance != nullptr) {
		NewEffect = Cast<AEffectDamageActor>(PoeInstance->EffectPooling->GetUnUseObject());
	}

	if (NewEffect == nullptr) {
		NewEffect = GetWorld()->SpawnActor<AEffectDamageActor>(AEffectDamageActor::StaticClass(), Location, FRotator::ZeroRotator);
		bPooling = false;
	}
	else NewEffect->SetActorLocation(Location);

	NewEffect->SetParticleSystem(SelectedEffect);
	NewEffect->Active();
	if(!bPooling && PoeInstance != nullptr) PoeInstance->EffectPooling->AddObject(NewEffect);

	FTimerHandle effectSpawnHandle;
	GetWorld()->GetTimerManager().SetTimer(effectSpawnHandle, [NewEffect]() {
			if (NewEffect != nullptr && ::IsValid(NewEffect)) {
				NewEffect->InActive();
			}
		}, 3.0f, false);
}

void APOECharacter::ChangeActive(int index)
{
	switch (index)
	{
	case 0:
		TEST_LOG("Change melee!");
		IsRangeAttack = false;
		break;
	case 1:
		SelectedEffect = LavaEffect;
		TEST_LOG("Change Fire!");
		IsRangeAttack = true;
		break;
	case 2:
	case 3:
		SelectedEffect = LightningEffect;
		TEST_LOG("Change Lightning!");
		IsRangeAttack = true;
		break;
	default:
		break;
	}
}

void APOECharacter::CalculateCoolTime()
{
	if (DashCoolTime > .0f) {
		DashCoolTime -= 1.0f;

		if (DashCoolTime <= .0f) {
			TEST_LOG("Can use dash now");
			DashCoolTime = .0f;
		}
	}
}



