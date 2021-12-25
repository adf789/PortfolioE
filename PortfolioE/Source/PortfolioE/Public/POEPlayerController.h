// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "PortfolioE.h"
#include "POENpcCharacter.h"
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
	APOEPlayerController();

	void DetectNPCOnCursor();

	UFUNCTION()
	bool IsDetectedNPC();

	UFUNCTION()
	void ShowNpcMenuWidget(APOENpcCharacter* npc);

	UFUNCTION()
	void HideNpcMenuWidget();

	void BindNpcMenuAction(APOENpcCharacter* npc);

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = UI)
	bool isShowWidget;


// ��� ����
protected:
	UPROPERTY()
	TWeakObjectPtr<class UPrimitiveComponent> outlineTarget;

	UPROPERTY(VisibleAnywhere, Category = UI)
	class UPOENpcMenuWidget* npcMenuWidget;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = UI)
	TSubclassOf<class UPOENpcMenuWidget> menuWidgetClass;

private:
	FTimerHandle detectTimer;
};
