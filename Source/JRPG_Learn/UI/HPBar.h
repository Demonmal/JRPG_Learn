// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HPBar.generated.h"

class AUnitBase;
class UTextBlock;
class UProgressBar;

UCLASS()
class JRPG_LEARN_API UHPBar : public UUserWidget
{
	GENERATED_BODY()

	public:

	UFUNCTION(BlueprintCallable)
	void SetUnit(AUnitBase* Unit);

	UFUNCTION(BlueprintCallable)
	void OnHPSetHandler(int HP);

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* HPText;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UProgressBar* HPBar;

	private:
	TWeakObjectPtr<AUnitBase> CachedUnit;
};
