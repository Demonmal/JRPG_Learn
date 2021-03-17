// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ActionTimeBar.generated.h"

class AUnitBase;
class UTextBlock;
class UProgressBar;
class UWidgetAnimation;

UCLASS()
class JRPG_LEARN_API UActionTimeBar : public UUserWidget
{
	GENERATED_BODY()

	public:

	UFUNCTION(BlueprintCallable)
	void SetUnit(AUnitBase* Unit);
	UFUNCTION(BlueprintCallable)
	void OnActionTimeAddedHandler(float ActionTime);
	UFUNCTION(BlueprintCallable)
	void OnTurnEndedHandler();
	UFUNCTION(BlueprintCallable)
	void OnUnitDiedHandler(AUnitBase* DiedUnit);
	UFUNCTION(BlueprintCallable)
	void ResetActionTimeBar();

	UPROPERTY(meta = (BindWidgetAnim))
	UWidgetAnimation* Fill;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* ReadyText;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UProgressBar* ActionTimeBar;

	private:
	TWeakObjectPtr<AUnitBase> CachedUnit;
};
