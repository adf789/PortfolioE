// Fill out your copyright notice in the Description page of Project Settings.

#include "POENpcCharacter.h"
#include "Components/WidgetComponent.h"
#include "Engine/StreamableManager.h"
#include "Templates/SharedPointer.h"
#include "POEGameInstance.h"
#include "POEPlayerController.h"

// Sets default values
APOENpcCharacter::APOENpcCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Compare NPC
	Tags.Add(TEXT("NPC"));

	/*MenuBarWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("MENUBARWIDGET"));
	MenuBarWidget->SetRelativeLocation(FVector(.0f, .0f, 300.0f));
	MenuBarWidget->SetWidgetSpace(EWidgetSpace::Screen);
	MenuBarWidget->SetupAttachment(GetMesh());

	static ConstructorHelpers::FClassFinder<UUserWidget>
		MENUBAR_WC(TEXT("/Game/POE/UIWidget/MenuWidget.MenuWidget_c"));
	if (MENUBAR_WC.Succeeded()) {
		MenuBarWidget->SetWidgetClass(MENUBAR_WC.Class);
		MenuBarWidget->SetDrawSize(FVector2D(100.0f, 150.0f));
	}

	OnShowMenuBar(false);*/
}

// Called when the game starts or when spawned
void APOENpcCharacter::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void APOENpcCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void APOENpcCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void APOENpcCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

}

void APOENpcCharacter::OnTalk()
{
	TEST_LOG("On Talk");
}

void APOENpcCharacter::OnAction()
{
	TEST_LOG("On Action");
}

void APOENpcCharacter::OnCancel()
{
	TEST_LOG("On Cancel");

	auto POEPlayerController = Cast<APOEPlayerController>(GetWorld()->GetFirstPlayerController());
	CHECKRETURN(POEPlayerController == nullptr);

	POEPlayerController->HideNpcMenuWidget();
}

