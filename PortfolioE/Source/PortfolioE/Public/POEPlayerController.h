// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "PortfolioE.h"
#include "GameFramework/PlayerController.h"
#include "POEPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class PORTFOLIOE_API APOEPlayerController : public APlayerController
{
	GENERATED_BODY()
	
// �Լ�
protected:
	virtual void SetupInputComponent() override;
	virtual void BeginPlay() override;
	
public:
	//virtual void PlayerTick(float DeltaTime) override;

	void DetectNPCOnCursor();

// ��� ����
private:
	UPROPERTY()
	TWeakObjectPtr<class UPrimitiveComponent> currentTarget;

	FTimerHandle detectTimer;
};
