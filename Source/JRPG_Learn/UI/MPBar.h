// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MPBar.generated.h"

class AUnitBase;
class UTextBlock;
class UProgressBar;

UCLASS()
class JRPG_LEARN_API UMPBar : public UUserWidget
{
	GENERATED_BODY()
	
	public:

	UFUNCTION(BlueprintCallable)
	void SetUnit(AUnitBase* Unit);

	UFUNCTION(BlueprintCallable)
	void OnMPSetHandler(int MP);

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* MPText;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UProgressBar* MPBar;

	private:
	TWeakObjectPtr<AUnitBase> CachedUnit;
};
