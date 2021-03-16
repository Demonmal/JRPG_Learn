// Fill out your copyright notice in the Description page of Project Settings.

#include "UsableItemBase.h"
#include "../../Controllers/JRPG_PlayerController.h"
#include "../../Battle/BattleController.h"
#include "../../Units/UnitBase.h"
#include "../../Units/PlayerUnits/PlayerUnitBase.h"

void AUsableItemBase::Use(ABattleController *BattleController_l, AJRPG_PlayerController *PlayerController_l,
                          bool bIsBattle, AUnitBase *Unit, TSubclassOf<APlayerUnitBase> PlayerUnitClass)
{
    BattleController = BattleController_l;
    PlayerController = PlayerController_l;
    PlayerController->OnUsableItemUsed(StaticClass(), 1);
    if(bIsBattle)
    {
        OnUsedInBattle(Unit, PlayerUnitClass);
    }
    else
    {
        OnUsedOutsideBattle(PlayerUnitClass);
    }
}

void AUsableItemBase::OnUsedInBattle(AUnitBase *Unit, TSubclassOf<APlayerUnitBase> PlayerUnitClass)
{
    if (OnItemUsed.IsBound())
    {
        OnItemUsed.Broadcast();
    }
}

void AUsableItemBase::OnUsedOutsideBattle(TSubclassOf<APlayerUnitBase> PlayerUnitClass)
{
    if (OnItemUsed.IsBound())
    {
        OnItemUsed.Broadcast();
    }
}