// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractionBattle.h"

void AInteractionBattle::RemoveBattle()
{
    EnemyUnits.Empty();
    PlayerUnits.Empty();
    if(OnBattleRemoved.IsBound())
    {
        OnBattleRemoved.Broadcast();
    }
}