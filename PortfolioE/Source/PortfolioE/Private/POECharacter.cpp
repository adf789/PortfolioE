// Fill out your copyright notice in the Description page of Project Settings.

#include "POECharacter.h"
#include "Components/InputComponent.h"
#include "POEPlayerController.h"
#include "POENpcCharacter.h"
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
		SK_SEVALOG(TEXT("/Game/ParagonSevarog/Characters/Heroes/Sevarog/Meshes/Sevarog.Sevarog"));
	if (SK_SEVALOG.Succeeded()) {
		GetMesh()->SetSkeletalMesh(SK_SEVALOG.Object);
		GetMesh()->SetWorldRotation(FRotationMatrix::MakeFromZ(GetActorUpVector()).Rotator());
	}
}

// Called when the game starts or when spawned
void APOECharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void APOECharacter::SetControlMode(EControlType ControlType)
{
	switch (ControlType) {
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
		GetCharacterMovement()->bOrientRotationToMovement = false;
		GetCharacterMovement()->bUseControllerDesiredRotation = false;
		GetCharacterMovement()->RotationRate = FRotator(.0f, 720.0f, .0f);
		break;
	}
	case EControlType::NPC: {
		break;
	}
	case EControlType::Monster: {
		break;
	}
	}
}

// Called every frame
void APOECharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (CheckMouseDrag) SetDestination();
}

// Called to bind functionality to input
void APOECharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	FInputActionBinding Pressed(TEXT("SetDestination"), EInputEvent::IE_Pressed);
	Pressed.ActionDelegate.GetDelegateForManualSet().BindLambda([this]() {
		if (poePlayerController != nullptr && poePlayerController->IsDetectedNPC()) {
			CheckNPC = true;
			return;
		}
		CheckMouseDrag = true;
		});
	FInputActionBinding Released(TEXT("SetDestination"), EInputEvent::IE_Released);
	Released.ActionDelegate.GetDelegateForManualSet().BindLambda([this]() {
		if (CheckNPC) ClickTarget();
		CheckMouseDrag = false;
		CheckNPC = false;
		});
	PlayerInputComponent->AddActionBinding(Pressed);
	PlayerInputComponent->AddActionBinding(Released);
}

void APOECharacter::PossessedBy(AController * NewController)
{
	Super::PossessedBy(NewController);

	poePlayerController = Cast<APOEPlayerController>(NewController);
	CHECKRETURN(poePlayerController == nullptr);
	if (IsPlayerControlled()) {
		SetControlMode(EControlType::Player);
	}
	else {

	}
}

void APOECharacter::SetDestination()
{
	CHECKRETURN(poePlayerController == nullptr);

	FHitResult hitResult;
	bool bResult = poePlayerController->GetHitResultUnderCursorByChannel(UEngineTypes::ConvertToTraceType(ECollisionChannel::ECC_GameTraceChannel1), true, hitResult);

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

	if (CheckNPC && !poePlayerController->IsDetectedNPC()) CheckNPC = false;
}

void APOECharacter::ClickTarget()
{
	CHECKRETURN(poePlayerController == nullptr);

	FHitResult hitResult;
	bool bResult = poePlayerController->GetHitResultUnderCursor(ECollisionChannel::ECC_WorldDynamic, true, hitResult);
	if (bResult) {
		AActor* actor = hitResult.GetActor();
		if (actor == nullptr || !actor->Tags.Contains(TEXT("NPC"))) {
			return;
		}

		APOENpcCharacter* character = Cast<APOENpcCharacter>(actor);
		if (character != nullptr) {
			character->OnShowMenuBar(true);
		}
	}
}



