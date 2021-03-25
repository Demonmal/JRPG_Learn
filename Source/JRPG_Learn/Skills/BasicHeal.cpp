// Fill out your copyright notice in the Description page of Project Settings.

#include "BasicHeal.h"
#include "Kismet/KismetSystemLibrary.h"
#include "../Units/UnitBase.h"
#include "../Battle/BattleController.h"
#include "../Controllers/JRPG_PlayerController.h"
#include "../Controllers/JRPG_FunctionLibrary.h"
#include "TimerManager.h"

void ABasicHeal::UseSkill()
{
    BattleController->GetCurrentTargetUnit()->Heal(HealAmount);
    PlayerController->PlaySound2DByTag(SoundTag);
    FTimerDelegate TimerCallback;
    FTimerHandle TimerHandle;
    TimerCallback.BindLambda([&]() {
        if (OnSkillUsed.IsBound())
        {
            OnSkillUsed.Broadcast();
        }
    });
    GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerCallback, DelayAfterUsage, false);
}

void ABasicHeal::UseBattleSkillInExplore(TSubclassOf<APlayerUnitBase> TargetUnit)
{
    FPlayerUnitData PlayerUnitData;
    if (PlayerController->TryGetUnitDataByPlayer(TargetUnit, PlayerUnitData))
    {
        int Level;
        FUnitStats UnitStats = UJRPG_FunctionLibrary::GetPlayerUnitStatsFromData(TargetUnit, PlayerUnitData, Level);
        PlayerUnitData.CurrentHP = PlayerUnitData.CurrentHP + HealAmount > UnitStats.MaxHP ? UnitStats.MaxHP : PlayerUnitData.CurrentHP + HealAmount;
    }
}