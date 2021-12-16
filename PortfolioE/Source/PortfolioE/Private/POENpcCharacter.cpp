// Fill out your copyright notice in the Description page of Project Settings.

#include "POENpcCharacter.h"


// Sets default values
APOENpcCharacter::APOENpcCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<USkeletalMesh>
		TEMP_SK(TEXT("SkeletalMesh'/Game/Mannequin/Character/Mesh/SK_Mannequin.SK_Mannequin'"));
	if (TEMP_SK.Succeeded()) {
		GetMesh()->SetSkeletalMesh(TEMP_SK.Object);
		GetMesh()->SetWorldLocation(FVector(.0f, .0f, -90.0f));
	}

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

