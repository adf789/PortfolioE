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
#include "WidgetComponent.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Engine/TextureRenderTarget2D.h"
#include "PaperSpriteComponent.h"
#include "PaperSprite.h"
#include "MyInventoryComponent.h"
#include "InventoryItem_Equipment.h"


// Sets default values
APOECharacter::APOECharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SPRINGARM"));
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("CAMERA"));
	MeleeCollision = CreateDefaultSubobject<UCapsuleComponent>(TEXT("MELEECOLLISION"));
	SpringArmForCapture = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmForCapture"));
	CaptureCamera = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("CaptureCamera"));
	ArrowSprite = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("ArrowSprite"));
	Inventory = CreateDefaultSubobject<UMyInventoryComponent>(TEXT("Inventory"));

	SpringArm->SetupAttachment(GetCapsuleComponent());
	Camera->SetupAttachment(SpringArm);
	SpringArmForCapture->SetupAttachment(GetCapsuleComponent());
	CaptureCamera->SetupAttachment(SpringArmForCapture);
	CaptureCamera->ShowFlags.SetSkeletalMeshes(false);
	CaptureCamera->ShowFlags.SetLighting(false);
	ArrowSprite->SetupAttachment(GetCapsuleComponent());
	ArrowSprite->SetWorldScale3D(FVector(.2f, .2f, .2f));
	ArrowSprite->SetWorldLocationAndRotation(FVector(.0f, .0f, 4000.0f), FRotator(.0f, .0f, 90.0f));
	ArrowSprite->SetOwnerNoSee(true);

	static ConstructorHelpers::FObjectFinder<USkeletalMesh>
		SK_SEVAROG(TEXT("/Game/ParagonSevarog/Characters/Heroes/Sevarog/Meshes/Sevarog.Sevarog"));
	if (SK_SEVAROG.Succeeded()) {
		GetMesh()->SetSkeletalMesh(SK_SEVAROG.Object);
		GetMesh()->SetWorldLocationAndRotation(FVector(.0f, .0f, -90.0f), FRotator(.0f, -120.0f, .0f));
	}

	static ConstructorHelpers::FObjectFinder<UMaterialInstanceConstant>
		MT_SMEAR(TEXT("/Game/ParagonSevarog/Characters/Heroes/Sevarog/Materials/M_Sevarog_Torso_2_Inst.M_Sevarog_Torso_2_Inst"));
	if (MT_SMEAR.Succeeded()) {
		SmearMaterialInstance = GetMesh()->CreateDynamicMaterialInstance(0, MT_SMEAR.Object);
	}

	static ConstructorHelpers::FClassFinder<UAnimInstance>
		SAVAROG_ANIM(TEXT("/Game/POE/Blueprints/Animations/SevarogAnim.SevarogAnim_c"));
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

	static ConstructorHelpers::FObjectFinder<UTextureRenderTarget2D>
		TEXTURE_RENDER_TARGET(TEXT("/Game/POE/Effects/Textures/MinimapRenderTexture.MinimapRenderTexture"));
	if (TEXTURE_RENDER_TARGET.Succeeded()) {
		CaptureCamera->TextureTarget = TEXTURE_RENDER_TARGET.Object;
	}

	static ConstructorHelpers::FObjectFinder<UPaperSprite>
		SPRITE_ARROW(TEXT("/Game/POE/Effects/Textures/T_TransparentArrow_Sprite.T_TransparentArrow_Sprite"));
	if (SPRITE_ARROW.Succeeded()) {
		ArrowSprite->SetSprite(SPRITE_ARROW.Object);
	}

	CharacterStatus->InitHPVale(10000.0f);
	CharacterStatus->InitAttackValue(500.0f);
	CharacterStatus->InitDefenceValue(100.0f);
}

void APOECharacter::MeleeAttack()
{
	if (CharacterState != ECharacterBehaviorState::ATTACKING) {
		//IsAttacking = true;
		CharacterState = ECharacterBehaviorState::ATTACKING;
		AlreadyAttackColiision = false;
		CharacterAnim->PlayAttackCombo(1);
	}
	else if(IsComboInput){
		IsComboInput = false;
		AlreadyAttackColiision = false;
		CurrentCombo = FMath::Clamp(CurrentCombo + 1, 2, MaxCombo);
		CharacterAnim->PlayAttackCombo(CurrentCombo);
	}
}

void APOECharacter::OnAnimMontageEnded(UAnimMontage* Montage, bool bInterrupted) {
	Super::OnAnimMontageEnded(Montage, bInterrupted);
	IsComboInput = false;
	if(IsSprinting) SmearMaterialInstance->SetScalarParameterValue(TEXT("Amount"), 0);
	IsSprinting = false;
	//IsCasting = false;
	CharacterState = ECharacterBehaviorState::IDLE;
	CurrentCombo = 1;
}

void APOECharacter::SetAttackType()
{
	MaxCombo = 3;
	IsRangeAttack = false;
}

void APOECharacter::CheckAttackCombo()
{
	IsComboInput = true;
}

bool APOECharacter::IsPlayingMontionAnything()
{
	return CharacterState == ECharacterBehaviorState::ATTACKING || IsSprinting;
}

// Called when the game starts or when spawned
void APOECharacter::BeginPlay()
{
	Super::BeginPlay();
	
	SetAttackType();

	GetWorld()->GetTimerManager().SetTimer(CoolTimeHandle, this, &APOECharacter::CalculateCoolTime, 1.0f, true);
	LoadInventoryData();
}

void APOECharacter::LoadInventoryData()
{
	UInventoryItem_Equipment TestItem1;
	TestItem1.SetDisplayName(FName(TEXT("TestItem1")));
	TestItem1.SetHaveCount(2);
	TestItem1.SetDescription(FText::FromString(TEXT("Desc1")));
	TestItem1.SetTextureId(0);

	UInventoryItem_Equipment TestItem2;
	TestItem1.SetDisplayName(FName(TEXT("TestItem2")));
	TestItem1.SetHaveCount(100);
	TestItem1.SetDescription(FText::FromString(TEXT("Desc2")));
	TestItem1.SetTextureId(1);

	UInventoryItem_Equipment TestItem3;
	TestItem1.SetDisplayName(FName(TEXT("TestItem3")));
	TestItem1.SetHaveCount(20);
	TestItem1.SetDescription(FText::FromString(TEXT("Desc3")));
	TestItem1.SetTextureId(2);

	Inventory->InsertItem(&TestItem1);
	Inventory->InsertItem(&TestItem2);
	Inventory->InsertItem(&TestItem3);
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
		LaunchCharacter(GetActorForwardVector() * 10000.0f, false, false);
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

		SpringArmForCapture->TargetArmLength = 20000.0f;
		SpringArmForCapture->SetRelativeRotation(FRotator(-90.0f, .0f, .0f));
		SpringArmForCapture->bUsePawnControlRotation = false;
		SpringArmForCapture->bInheritPitch = false;
		SpringArmForCapture->bInheritRoll = false;
		SpringArmForCapture->bInheritYaw = false;
		SpringArmForCapture->bDoCollisionTest = false;
		CaptureCamera->ProjectionType = ECameraProjectionMode::Orthographic;
		CaptureCamera->OrthoWidth = 1500.0f;
	}

	POEPlayerController = Cast<APOEPlayerController>(NewController);
	CHECKRETURN(POEPlayerController == nullptr);
}

void APOECharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	CharacterAnim = Cast<UPOECharacterAnimInstance>(AnimInstance);
	CHECKRETURN(CharacterAnim == nullptr);

	CharacterAnim->OnNextComboCheck.AddUObject(this, &APOECharacter::CheckAttackCombo);

	GetCharacterMovement()->RotationRate = FRotator(.0f, 720.0f, .0f);
	GetCharacterMovement()->MaxWalkSpeed = 450.0f;
}

void APOECharacter::CheckMeleeAttackCollision()
{
	Super::CheckMeleeAttackCollision();

	if (AlreadyAttackColiision) return;

	FVector startLocation = GetMesh()->GetSocketLocation(TEXT("Weapon_TriggerStart"));
	FVector endLocation = GetMesh()->GetSocketLocation(TEXT("Weapon_TriggerEnd"));

	FHitResult HitResult;
	FCollisionQueryParams Params(NAME_None, false, this);
	bool bResult = GetWorld()->SweepSingleByChannel(
		HitResult,
		startLocation,
		endLocation,
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel2,
		FCollisionShape::MakeSphere(30.0f),
		Params
	);

#if ENABLE_DRAW_DEBUG
	DrawDebugCylinder(GetWorld(),
		startLocation,
		endLocation,
		30.0f,
		10,
		bResult ? FColor::Green : FColor::Red,
		false,
		0.1f);
#endif

	if (bResult) {
		APOECharacter_Base* TargetCharacter = Cast<APOECharacter_Base>(HitResult.GetActor());
		if (TargetCharacter != nullptr) {
			FDamageEvent DamageEvent;
			TargetCharacter->TakeDamage(CharacterStatus->AttackValue, DamageEvent, GetController(), this);
			AlreadyAttackColiision = true;
		}
	}
}

float APOECharacter::TakeDamage(float DamageAmount, FDamageEvent const & DamageEvent, AController * EventInstigator, AActor * DamageCauser)
{
	float TakeDamageValue = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	StatusWidget->SetHiddenInGame(true);
	return TakeDamageValue;
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
	if (CharacterState == ECharacterBehaviorState::ATTACKING) return;
	//IsCasting = true;
	CharacterState = ECharacterBehaviorState::ATTACKING;
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



