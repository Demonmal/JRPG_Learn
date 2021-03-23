// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyUnitUI.h"
#include "../Units/EnemyUnits/EnemyUnitBase.h"
#include "../Controllers/JRPG_PlayerController.h"
#include "../Battle/BattleController.h"
#include "HPBar.h"
#include "ActionTimeBar.h"

void UEnemyUnitUI::Init(AEnemyUnitBase *EnemyUnit, ABattleController *BattleController)
{
    CurrentEnemyUnit = TWeakObjectPtr<AEnemyUnitBase>(EnemyUnit);
    BP_HPBar->SetUnit(EnemyUnit);
    // ETurnType TurnType = BattleController->GetTurnType();
    // if(TurnType == ETurnType::ActionTime)
    // {
    //     BP_ActionTimeBar->SetUnit(EnemyUnit);
    // }
    // else
    // {
    //     BP_ActionTimeBar->SetVisibility(ESlateVisibility::Collapsed);
    // }
}