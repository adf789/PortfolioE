// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "PortfolioE.h"
#include "GameFramework/Character.h"
#include "POENpcCharacter.generated.h"

UCLASS()
class PORTFOLIOE_API APOENpcCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APOENpcCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void OnLoadAssetComplete();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void PostInitializeComponents() override;

	UFUNCTION()
	void OnShowMenuBar(bool isActive);

public:
	UPROPERTY(VisibleAnywhere, Category = UI)
	class UWidgetComponent* MenuBarWidget;

protected:
	UPROPERTY()
	FSoftObjectPath MenuWidgetPath;

	UPROPERTY()
	FSoftObjectPath MeshPath;	

	TSharedPtr<struct FStreamableHandle> AssetStreamingHandles;
};
