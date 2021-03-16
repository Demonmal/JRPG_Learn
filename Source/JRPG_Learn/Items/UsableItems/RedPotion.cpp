// Fill out your copyright notice in the Description page of Project Settings.


#include "RedPotion.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "../../Controllers/JRPG_FunctionLibrary.h"
#include "../../Controllers/JRPG_PlayerController.h"
#include "../../Battle/BattleController.h"
#include "../../Units/UnitBase.h"
#include "../../Units/PlayerUnits/PlayerUnitBase.h"

void ARedPotion::OnUsedInBattle(AUnitBase *Unit, TSubclassOf<APlayerUnitBase> PlayerUnitClass)
{
    Unit->PlayAnimationMontage(Unit->GetItemUseAnimMontage());
    Unit->Heal(UKismetMathLibrary::FTrunc(Unit->GetMaxHP() * HealPercent));
    FLatentActionInfo LatentActionInfo;
    UKismetSystemLibrary::Delay(GetWorld(), 1.5f, LatentActionInfo);
    AUsableItemBase::OnUsedInBattle(Unit, PlayerUnitClass);
}

void ARedPotion::OnUsedOutsideBattle(TSubclassOf<APlayerUnitBase> PlayerUnitClass)
{
    FPlayerUnitData UnitData;
    if(PlayerController->TryGetUnitDataByPlayer(PlayerUnitClass, UnitData))
    {
        int Level;
        FUnitStats Stats = UJRPG_FunctionLibrary::GetPlayerUnitStatsFromData(PlayerUnitClass, UnitData, Level);
        float HealthToRegen = UKismetMathLibrary::FTrunc(Stats.MaxHP * HealPercent);
        float NewHealth = UnitData.CurrentHP + HealthToRegen;
        UnitData.CurrentHP = Stats.MaxHP > NewHealth ? NewHealth : Stats.MaxHP;
    }
    AUsableItemBase::OnUsedOutsideBattle(PlayerUnitClass);
}