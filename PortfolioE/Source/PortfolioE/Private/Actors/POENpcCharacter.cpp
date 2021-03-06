// Description
// 플레이어블 캐릭터 외 NPC, 몬스터의 기능을 담당하는 클래스

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
	
}

void APOENpcCharacter::OnAction()
{
	
}

void APOENpcCharacter::OnCancel()
{
	auto POEPlayerController = Cast<APOEPlayerController>(GetWorld()->GetFirstPlayerController());
	CHECKRETURN(POEPlayerController == nullptr);

	POEPlayerController->HideNpcMenuWidget();
}

