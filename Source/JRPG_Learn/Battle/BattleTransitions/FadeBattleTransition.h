// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BattleTransitionBase.h"
#include "FadeBattleTransition.generated.h"

class USceneCaptureComponent2D;
class UFadeTransitionUI;

UCLASS()
class JRPG_LEARN_API AFadeBattleTransition : public ABattleTransitionBase
{
	GENERATED_BODY()
	
	public:
	AFadeBattleTransition();

	virtual void StartTransition() override;

	protected:

	UPROPERTY(VisibleDefaultsOnly)
	USceneCaptureComponent2D *SceneCaptureComponent;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UFadeTransitionUI> FadeTransitionUIClass;

	UPROPERTY()
	UFadeTransitionUI *FadeTransitionUI;
};
