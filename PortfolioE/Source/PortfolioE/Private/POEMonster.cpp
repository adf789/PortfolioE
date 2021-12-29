// Fill out your copyright notice in the Description page of Project Settings.

#include "POEMonster.h"


// Sets default values
APOEMonster::APOEMonster()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<USkeletalMesh>
		SM_GRUX(TEXT("/Game/ParagonGrux/Characters/Heroes/Grux/Meshes/Grux.Grux"));
	if (SM_GRUX.Succeeded()) {
		GetMesh()->SetSkeletalMesh(SM_GRUX.Object);
	}
}

// Called when the game starts or when spawned
void APOEMonster::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APOEMonster::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void APOEMonster::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

