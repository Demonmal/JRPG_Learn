// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Widget.h"
#include "TargetIcon.generated.h"

/**
 * 
 */
UCLASS()
class JRPG_LEARN_API UTargetIcon : public UUserWidget
{
	GENERATED_BODY()
	
	public:

	UFUNCTION(BlueprintCallable)
	void SetTargetIconVisibility(bool bIsVisible);

	UPROPERTY(meta = (BindWidgetAnim))
	class UWidgetAnimation* MoveAnimation;
};
