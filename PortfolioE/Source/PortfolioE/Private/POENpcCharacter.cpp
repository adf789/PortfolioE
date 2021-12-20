// Fill out your copyright notice in the Description page of Project Settings.

#include "POENpcCharacter.h"
#include "Components/WidgetComponent.h"
#include "Engine/StreamableManager.h"
#include "Templates/SharedPointer.h"
#include "POEGameInstance.h"

// Sets default values
APOENpcCharacter::APOENpcCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Compare NPC
	Tags.Add(TEXT("NPC"));

	MenuBarWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("MENUBARWIDGET"));
	MenuBarWidget->SetRelativeLocation(FVector(.0f, .0f, 300.0f));
	MenuBarWidget->SetWidgetSpace(EWidgetSpace::Screen);
	MenuBarWidget->SetupAttachment(GetMesh());

	OnShowMenuBar(false);

	MenuWidgetPath = FSoftObjectPath(TEXT("/Game/POE/UIWidget/MenuWidget.MenuWidget_C"));
}

// Called when the game starts or when spawned
void APOENpcCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	auto POEGameInstance = Cast<UPOEGameInstance>(GetGameInstance());
	CHECKRETURN(POEGameInstance == nullptr);
	AssetStreamingHandles = POEGameInstance->StreamableManager.RequestAsyncLoad(MenuWidgetPath, FStreamableDelegate::CreateUObject(this, &APOENpcCharacter::OnLoadAssetComplete));
}

void APOENpcCharacter::OnLoadAssetComplete()
{
	UUserWidget* widgetLoaded = Cast<UUserWidget>(AssetStreamingHandles->GetLoadedAsset());
	AssetStreamingHandles->Reset();
	CHECKRETURN(widgetLoaded == nullptr);

	MenuBarWidget->SetWidget(widgetLoaded);
	MenuBarWidget->SetDrawSize(FVector2D(100.0f, 150.0f));
	/*static ConstructorHelpers::FClassFinder<UUserWidget>
		MENUBAR_WC(TEXT("/Game/POE/UIWidget/MenuWidget.MenuWidget_c"));
	if (MENUBAR_WC.Succeeded()) {
		MenuBarWidget->SetWidgetClass(MENUBAR_WC.Class);
		MenuBarWidget->SetDrawSize(FVector2D(100.0f, 150.0f));
	}*/
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

void APOENpcCharacter::OnShowMenuBar(bool isActive)
{
	MenuBarWidget->SetHiddenInGame(!isActive);
}

