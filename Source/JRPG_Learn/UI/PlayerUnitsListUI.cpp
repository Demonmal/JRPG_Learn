// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerUnitsListUI.h"
#include "Components/HorizontalBox.h" 
#include "PlayerUnitUI.h"
#include "../Battle/BattleController.h"
#include "../Units/PlayerUnits/PlayerUnitBase.h"

void UPlayerUnitsListUI::SetPlayerUnitUIList(TArray<APlayerUnitBase *> PlayerUnits, ABattleController *BattleController)
{
    for(auto PlayerUnit : PlayerUnits)
    {
        DisplayUnitUI(PlayerUnit, BattleController);
    }
}

void UPlayerUnitsListUI::DisplayUnitUI(APlayerUnitBase *PlayerUnit, ABattleController *BattleController)
{
    UPlayerUnitUI* PlayerUnitUI = CreateWidget<UPlayerUnitUI>(GetWorld(), PlayerUnitUIClass);
    PlayerUnitUI->PlayerUnit = PlayerUnit;
    PlayerUnitList->AddChildToHorizontalBox(PlayerUnitUI);
    PlayerUnitUI->Init(BattleController);
}