// Fill out your copyright notice in the Description page of Project Settings.

#include "POEWeapon.h"
#include "POECharacter_Base.h"


// Sets default values
APOEWeapon::APOEWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void APOEWeapon::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APOEWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

APOECharacter_Base * APOEWeapon::GetWeaponOwner()
{
	return WeaponOwner;
}

