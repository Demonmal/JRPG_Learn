// Fill out your copyright notice in the Description page of Project Settings.


#include "BluePotion.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "../../Controllers/JRPG_FunctionLibrary.h"
#include "../../Controllers/JRPG_PlayerController.h"
#include "../../Battle/BattleController.h"
#include "../../Units/UnitBase.h"
#include "../../Units/PlayerUnits/PlayerUnitBase.h"
#include "TimerManager.h"

void ABluePotion::OnUsedInBattle(AUnitBase *Unit, TSubclassOf<APlayerUnitBase> PlayerUnitClass)
{
    Unit->PlayAnimMontage(Unit->GetItemUseAnimMontage());
    Unit->IncreaseMP(UKismetMathLibrary::FTrunc(Unit->GetMaxMP() * ManaRestorePercent));
    FTimerDelegate TimerCallback;
    FTimerHandle TimerHandle;
    TimerCallback.BindLambda([&]() {
        AUsableItemBase::OnUsedInBattle(Unit, PlayerUnitClass);
    });
    GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerCallback, 1.5f, false);
}

void ABluePotion::OnUsedOutsideBattle(TSubclassOf<APlayerUnitBase> PlayerUnitClass)
{
    FPlayerUnitData UnitData;
    if(PlayerController->TryGetUnitDataByPlayer(PlayerUnitClass, UnitData))
    {
        int Level;
        FUnitStats Stats = UJRPG_FunctionLibrary::GetPlayerUnitStatsFromData(PlayerUnitClass, UnitData, Level);
        float ManaToRestore = UKismetMathLibrary::FTrunc(Stats.MaxMP * ManaRestorePercent);
        float NewMana = UnitData.CurrentMP + ManaToRestore;
        UnitData.CurrentHP = Stats.MaxMP > NewMana ? NewMana : Stats.MaxMP;
    }
    AUsableItemBase::OnUsedOutsideBattle(PlayerUnitClass);
}
