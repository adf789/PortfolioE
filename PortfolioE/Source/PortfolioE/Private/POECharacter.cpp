// Fill out your copyright notice in the Description page of Project Settings.

#include "POECharacter.h"
#include "Components/InputComponent.h"
#include "POEPlayerController.h"
#include "POENpcCharacter.h"
#include "POECharacterAnimInstance.h"
#include "DrawDebugHelpers.h"


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

	static ConstructorHelpers::FClassFinder<UAnimInstance>
		SAVAROG_ANIM(TEXT("/Game/POE/Animations/SevarogAnim.SevarogAnim_C"));
	if (SAVAROG_ANIM.Succeeded()) {
		GetMesh()->SetAnimInstanceClass(SAVAROG_ANIM.Class);
	}
}

void APOECharacter::MeleeAttack(FVector Direction)
{
	if (!IsAttacking) {
		IsAttacking = true;
		CharacterAnim->PlayMeleeAttack();
		//GetCharacterMovement()->AddImpulse(GetActorForwardVector() * 300.0f * GetMesh()->GetMass(), true);
	}
	else if(IsComboInput){
		IsComboInput = false;
		CurrentCombo = FMath::Clamp(CurrentCombo + 1, 1, MaxCombo);
		CharacterAnim->JumpToAttackMeleeCombo(CurrentCombo);
		//GetCharacterMovement()->AddImpulse(GetActorForwardVector() * 300.0f);
	}
}

void APOECharacter::OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted) {
	IsAttacking = false;
	IsComboInput = false;
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

// Called when the game starts or when spawned
void APOECharacter::BeginPlay()
{
	Super::BeginPlay();
	
	SetAttackType();
}

void APOECharacter::SetControlMode(EControlType controlType)
{
	switch (controlType) {
	case EControlType::Player: {
		SpringArm->TargetArmLength = 1500.0f;
		SpringArm->SetRelativeRotation(FRotator(-45.0f, .0f, .0f));
		SpringArm->bUsePawnControlRotation = false;
		SpringArm->bInheritPitch = false;
		SpringArm->bInheritRoll = false;
		SpringArm->bInheritYaw = false;
		SpringArm->bDoCollisionTest = false; // 카메라가 장애물을 뚫지않게하기위함
		bUseControllerRotationYaw = false;
		bUseControllerRotationPitch = true;
		bUseControllerRotationRoll = true;
		GetCharacterMovement()->bOrientRotationToMovement = true;
		GetCharacterMovement()->bUseControllerDesiredRotation = false;
		GetCharacterMovement()->RotationRate = FRotator(.0f, 720.0f, .0f);
		break;
	}
	case EControlType::Monster: {
		break;
	}
	}
}

void APOECharacter::ActiveSkill()
{
	CHECKRETURN(POEPlayerController == nullptr);

	FHitResult hitResult;
	bool bResult = POEPlayerController->GetHitResultUnderCursorByChannel(UEngineTypes::ConvertToTraceType(ECollisionChannel::ECC_GameTraceChannel1), true, hitResult);

	if (!bResult) return;

	FVector Direction = hitResult.Location - GetActorLocation();
	Direction.Z = .0f;
	FRotator Rot = FRotationMatrix::MakeFromX(Direction).Rotator();

	if (!IsAttacking) {
		SetActorRotation(Rot);
		UNavigationSystem::SimpleMoveToLocation(GetController(), GetActorLocation());
	}

	MeleeAttack(Direction);
}

// Called every frame
void APOECharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (CheckMouseDrag && !IsAttacking) SetDestination();
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
	PlayerInputComponent->BindAction(TEXT("ActiveSkill"), EInputEvent::IE_Pressed, this, &APOECharacter::ActiveSkill);
}

void APOECharacter::PossessedBy(AController * NewController)
{
	Super::PossessedBy(NewController);

	POEPlayerController = Cast<APOEPlayerController>(NewController);
	CHECKRETURN(POEPlayerController == nullptr);
	if (IsPlayerControlled()) {
		SetControlMode(EControlType::Player);
	}
	else {

	}
}

void APOECharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	CharacterAnim = Cast<UPOECharacterAnimInstance>(GetMesh()->GetAnimInstance());
	CHECKRETURN(CharacterAnim == nullptr);

	CharacterAnim->OnMontageEnded.AddDynamic(this, &APOECharacter::OnAttackMontageEnded);
	CharacterAnim->OnAttackCollision.AddUObject(this, &APOECharacter::CheckAttackRange);
	CharacterAnim->OnNextComboCheck.AddUObject(this, &APOECharacter::CheckAttackCombo);
}

void APOECharacter::SetDestination()
{
	CHECKRETURN(POEPlayerController == nullptr);

	FHitResult hitResult;
	bool bResult = POEPlayerController->GetHitResultUnderCursorByChannel(UEngineTypes::ConvertToTraceType(ECollisionChannel::ECC_GameTraceChannel1), true, hitResult);

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



