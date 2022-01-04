// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "PortfolioE.h"
#include "GameFramework/Actor.h"
//#include "POECharacter_Base.h"
#include "POEWeapon.generated.h"

UCLASS()
class PORTFOLIOE_API APOEWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APOEWeapon();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	class APOECharacter_Base* GetWeaponOwner();

protected:
	class APOECharacter_Base* WeaponOwner;

public:
	UPROPERTY(VisibleAnywhere, Category = Weapon)
	USkeletalMeshComponent* WeaponMesh;
	
};
