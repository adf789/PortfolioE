// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "PortfolioE.h"
#include "Components/SceneComponent.h"
#include "UIScreenInteraction.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PORTFOLIOE_API UUIScreenInteraction : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UUIScreenInteraction();

	virtual void DestroyComponent(bool bPromoteChildren = false) override;
	void ShowPanel(EUIPanelName ScreenName);
	void ClosePanel(EUIPanelName ScreenName);
	class UUserWidget* GetPanel(EUIPanelName ScreenName);
	int PanelCount();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	TMap<EUIPanelName, class UUserWidget*> UIPanels;

private:
	TSharedPtr<struct FStreamableHandle> AssetStreamingHandle;
};
