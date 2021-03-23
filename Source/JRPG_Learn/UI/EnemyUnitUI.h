// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EnemyUnitUI.generated.h"

class AEnemyUnitBase;
class ABattleController;

UCLASS()
class JRPG_LEARN_API UEnemyUnitUI : public UUserWidget
{
	GENERATED_BODY()

	public:

	UFUNCTION(BlueprintCallable)
	void Init(AEnemyUnitBase *EnemyUnit, ABattleController *BattleController);

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UHPBar* BP_HPBar;
	// UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	// class UActionTimeBar* BP_ActionTimeBar;
	
	private:
	TWeakObjectPtr<AEnemyUnitBase> CurrentEnemyUnit;
};
