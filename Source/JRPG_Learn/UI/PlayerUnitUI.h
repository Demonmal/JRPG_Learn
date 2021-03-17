// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerUnitUI.generated.h"

class ABattleController;
class UImage;

UCLASS()
class JRPG_LEARN_API UPlayerUnitUI : public UUserWidget
{
	GENERATED_BODY()

	public:

	UFUNCTION(BlueprintCallable)
	void Init(ABattleController *BattleController);
	UFUNCTION(BlueprintCallable)
	void OnTurnStartedHandler();
	UFUNCTION(BlueprintCallable)
	void OnTurnEndedHandler();
	UFUNCTION(BlueprintCallable)
	void OnUnitDiedHandler(AUnitBase* DiedUnit);

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UImage* UnitImage;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UImage* Background;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UHPBar* BP_HPBar;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UMPBar* BP_MPBar;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UActionTimeBar* BP_ActionTimeBar;

	TWeakObjectPtr<class APlayerUnitBase> PlayerUnit;
};
