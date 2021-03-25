// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DamageText.generated.h"

UCLASS()
class JRPG_LEARN_API ADamageText : public AActor
{
	GENERATED_BODY()

	public:

	ADamageText();

	void CreateTextWidget(int Amount, class AUnitBase *TargetUnit, bool bIsHeal = false, bool bIsMPRestore = false);

protected:

	UPROPERTY(VisibleDefaultsOnly)
	class UWidgetComponent* DamageTextUIWidget;
	UPROPERTY()
	class UDamageTextUI* DamageTextUI;

	const float BounceAnimationWaitTime = 0.5f;
};
