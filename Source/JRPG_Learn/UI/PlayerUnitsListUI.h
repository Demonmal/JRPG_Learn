// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerUnitsListUI.generated.h"

class APlayerUnitBase;
class ABattleController;

UCLASS()
class JRPG_LEARN_API UPlayerUnitsListUI : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void SetPlayerUnitUIList(TArray<APlayerUnitBase *> PlayerUnits, ABattleController *BattleController);
	UFUNCTION(BlueprintCallable)
	void DisplayUnitUI(APlayerUnitBase *PlayerUnit, ABattleController *BattleController);

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TSubclassOf<class UPlayerUnitUI> PlayerUnitUIClass; 

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UHorizontalBox* PlayerUnitList;
};
