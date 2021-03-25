// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DamageTextUI.generated.h"

class AUnitBase;
class UWidgetAnimation;

UCLASS()
class JRPG_LEARN_API UDamageTextUI : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void SetDamage(int Amount_l, AUnitBase *TargetUnit_l, bool bIsHeal_l, bool bIsMPRestore_l);

	UFUNCTION(BlueprintImplementableEvent)
	void SetDamageColor();
	UFUNCTION(BlueprintImplementableEvent)
	void SetDamageText();

	void UnbindAllFromAppearAnimation();

	UPROPERTY(BlueprintReadWrite)
	AUnitBase *TargetUnit;
	UPROPERTY(BlueprintReadWrite)
	int Amount;
	UPROPERTY(BlueprintReadWrite)
	bool bIsHeal;
	UPROPERTY(BlueprintReadWrite)
	bool bIsMPRestore;

protected:
	UPROPERTY(meta = (BindWidgetAnim))
	UWidgetAnimation *Appear;
	UPROPERTY(meta = (BindWidgetAnim))
	UWidgetAnimation *Bounce;
};
