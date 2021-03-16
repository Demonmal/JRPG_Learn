// Fill out your copyright notice in the Description page of Project Settings.

#include "JRPG_FunctionLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"
#include "UObject/ConstructorHelpers.h"
#include "../Units/PlayerUnits/PlayerUnitBase.h"
#include "../Items/Equipment/EquipmentBase.h"
#include "../Items/Equipment/Weapons/WeaponBase.h"
#include "../Items/Equipment/Armors/ArmorBase.h"
#include "../Items/Equipment/Helms/HelmBase.h"
#include "../Items/Equipment/Boots/BootsBase.h"
#include "../Items/Equipment/Shields/ShieldBase.h"

FUnitStats UJRPG_FunctionLibrary::GetUnitStats(int &Level, const int &MaxLevel, const FUnitStats &InitialStats, const FUnitStats &FirstLevelStats, const FUnitStats &LastLevelStats)
{
    int HPStat = GetIntStat(Level, MaxLevel, InitialStats.MaxHP, FirstLevelStats.MaxHP, LastLevelStats.MaxHP);
    int MPStat = GetIntStat(Level, MaxLevel, InitialStats.MaxMP, FirstLevelStats.MaxMP, LastLevelStats.MaxMP);
    int MinAttackStat = GetIntStat(Level, MaxLevel, InitialStats.MinAttack, FirstLevelStats.MinAttack, LastLevelStats.MinAttack);
    int MaxAttackStat = GetIntStat(Level, MaxLevel, InitialStats.MaxAttack, FirstLevelStats.MaxAttack, LastLevelStats.MaxAttack);
    int DefenseStat = GetIntStat(Level, MaxLevel, InitialStats.Defense, FirstLevelStats.Defense, LastLevelStats.Defense);
    int SpeedStat = GetIntStat(Level, MaxLevel, InitialStats.Speed, FirstLevelStats.Speed, LastLevelStats.Speed);
    int HitStat = GetIntStat(Level, MaxLevel, InitialStats.Hit, FirstLevelStats.Hit, LastLevelStats.Hit);
    int MinMagicalAttackStat = GetIntStat(Level, MaxLevel, InitialStats.MinMagicalAttack, FirstLevelStats.MinMagicalAttack, LastLevelStats.MinMagicalAttack);
    int MaxMagicalAttackStat = GetIntStat(Level, MaxLevel, InitialStats.MaxMagicalAttack, FirstLevelStats.MaxMagicalAttack, LastLevelStats.MaxMagicalAttack);
    int MagicDefenseStat = GetIntStat(Level, MaxLevel, InitialStats.MagicDefense, FirstLevelStats.MagicDefense, LastLevelStats.MagicDefense);
    int ActionTimeStat = GetActionTimeStat(Level, MaxLevel, FirstLevelStats.ActionTime, LastLevelStats.ActionTime);
    FUnitStats Result;
    Result.MaxHP = HPStat;
    Result.MaxMP = MPStat;
    Result.MinAttack = MinAttackStat;
    Result.MaxAttack = MaxAttackStat;
    Result.Defense = DefenseStat;
    Result.Speed = SpeedStat;
    Result.Hit = HitStat;
    Result.MinMagicalAttack = MinMagicalAttackStat;
    Result.MaxMagicalAttack = MaxMagicalAttackStat;
    Result.MagicDefense = MagicDefenseStat;
    Result.ActionTime = ActionTimeStat;
    return Result;
}

int UJRPG_FunctionLibrary::GetIntStat(int &Level, const int &MaxLevel, const int &InitialStat, const int &FirstLevelStat, const int &LastLevelStat)
{
    float ClampedLevel = UKismetMathLibrary::MapRangeClamped(Level, 1.f, MaxLevel, FirstLevelStat, LastLevelStat);
    float RoundedLevel = UKismetMathLibrary::FTrunc(ClampedLevel);
    return InitialStat + RoundedLevel;
}

float UJRPG_FunctionLibrary::GetActionTimeStat(int &Level, const int &MaxLevel, const float &FirstLevelStat, const float &LastLevelStat)
{
    float ClampedLevel = UKismetMathLibrary::MapRangeClamped(Level, 1.f, MaxLevel, FirstLevelStat, LastLevelStat);
    ClampedLevel = UKismetMathLibrary::Max(0.1f, ClampedLevel) * 100;
    float RoundedLevel = UKismetMathLibrary::Round(ClampedLevel) * 0.01f;
    return RoundedLevel;
}

void UJRPG_FunctionLibrary::GetEquipmentSkills(const FPlayerUnitData &Data, TArray<TSubclassOf<ABattleSkillBase>> &EquipmentBattleSkills,
                                               TArray<TSubclassOf<AExploreSkillBase>> &EquipmentExploreSkills)
{
    AppendSkillsFromEquipment(Data.Weapon, EquipmentBattleSkills, EquipmentExploreSkills);
    AppendSkillsFromEquipment(Data.Armor, EquipmentBattleSkills, EquipmentExploreSkills);
    AppendSkillsFromEquipment(Data.Shield, EquipmentBattleSkills, EquipmentExploreSkills);
    AppendSkillsFromEquipment(Data.Helm, EquipmentBattleSkills, EquipmentExploreSkills);
    AppendSkillsFromEquipment(Data.Boots, EquipmentBattleSkills, EquipmentExploreSkills);
}

void UJRPG_FunctionLibrary::AppendSkillsFromEquipment(TSubclassOf<AEquipmentBase> EquipmentClass, 
                                                      TArray<TSubclassOf<ABattleSkillBase>> &EquipmentBattleSkills,
                                                      TArray<TSubclassOf<AExploreSkillBase>> &EquipmentExploreSkills)
{
    if (UKismetSystemLibrary::IsValidClass(EquipmentClass))
    {
        AEquipmentBase* Equipment = Cast<AEquipmentBase>(EquipmentClass->GetDefaultObject());
        EquipmentBattleSkills.Append(Equipment->BatlleSkills);
        EquipmentExploreSkills.Append(Equipment->ExploreSkills);
    }
}

FUnitStats UJRPG_FunctionLibrary::GetEquipmentStats(const FPlayerUnitData &Data)
{
    FUnitStats WeaponStats = GetEquipmentStat(Data.Weapon);
    FUnitStats ArmorStats = GetEquipmentStat(Data.Armor);
    FUnitStats ShieldStats = GetEquipmentStat(Data.Shield);
    FUnitStats HelmStats = GetEquipmentStat(Data.Helm);
    FUnitStats BootsStats = GetEquipmentStat(Data.Boots);
    FUnitStats Result = WeaponStats + ArmorStats + ShieldStats + HelmStats + BootsStats;
    return Result;
}

FUnitStats UJRPG_FunctionLibrary::GetEquipmentStat(TSubclassOf<AEquipmentBase> EquipmentClass)
{
    FUnitStats Result;
    if (UKismetSystemLibrary::IsValidClass(EquipmentClass))
    {
        Result = Cast<AEquipmentBase>(EquipmentClass->GetDefaultObject())->Stats;
    }
    return Result;
}

void UJRPG_FunctionLibrary::MakeAbsoluteStats(FUnitStats &Stats)
{
    Stats.MaxHP = UKismetMathLibrary::Max(Stats.MaxHP, 1);
    Stats.MaxMP = UKismetMathLibrary::Max(Stats.MaxMP, 1);
    Stats.MinAttack = UKismetMathLibrary::Max(Stats.MinAttack, 1);
    Stats.MaxAttack = UKismetMathLibrary::Max(Stats.MaxAttack, 1);
    Stats.Defense = UKismetMathLibrary::Max(Stats.Defense, 1);
    Stats.Speed = UKismetMathLibrary::Max(Stats.Speed, 1);
    Stats.Hit = UKismetMathLibrary::Max(Stats.Hit, 1);
    Stats.MinMagicalAttack = UKismetMathLibrary::Max(Stats.MinMagicalAttack, 1);
    Stats.MaxMagicalAttack = UKismetMathLibrary::Max(Stats.MaxMagicalAttack, 1);
    Stats.MagicDefense = UKismetMathLibrary::Max(Stats.MagicDefense, 1);
    Stats.ActionTime = UKismetMathLibrary::FMax(Stats.ActionTime, 0.1f);
}

FUnitStats UJRPG_FunctionLibrary::GetPlayerUnitStatsFromData(TSubclassOf<APlayerUnitBase> UnitClass, const FPlayerUnitData &Data, int &Level)
{
    FUnitStats ResultStats;
    if(UKismetSystemLibrary::IsValidClass(UnitClass))
    {
        APlayerUnitBase* Unit = Cast<APlayerUnitBase>(UnitClass->GetDefaultObject());
        Level = GetLevel(Data.Exp, Unit->GetExpExponent(), Unit->GetMaxLevel());
        FUnitStats Stats = GetUnitStats(Level, Unit->GetMaxLevel(), Unit->GetInitialStats(), Unit->GetFirstLevelStats(), Unit->GetLastLevelStats());
        FUnitStats EquipmentStats = GetEquipmentStats(Data);
        ResultStats = Stats + EquipmentStats;
        MakeAbsoluteStats(ResultStats);
    }
    return ResultStats;
}

int UJRPG_FunctionLibrary::GetLevel(const int &Exp, const float &Exponent, const int &MaxLevel)
{
    int Level = UKismetMathLibrary::FFloor(Exponent + UKismetMathLibrary::Sqrt((Exponent * Exponent) + ((Exponent * 4) * Exp))) / (Exponent * 2);
    Level = UKismetMathLibrary::FTrunc(Level);
    return Level <= MaxLevel ? Level : MaxLevel;
}

TArray<UClass *> UJRPG_FunctionLibrary::GetAllEquipmentClasses(const FPlayerUnitData &Data)
{
    TArray<UClass *> EquipmentClasses;
    if (UKismetSystemLibrary::IsValidClass(Data.Weapon))
    {
        EquipmentClasses.Add(Data.Weapon);
    }
    if (UKismetSystemLibrary::IsValidClass(Data.Armor))
    {
        EquipmentClasses.Add(Data.Armor);
    }
    if (UKismetSystemLibrary::IsValidClass(Data.Shield))
    {
        EquipmentClasses.Add(Data.Shield);
    }
    if (UKismetSystemLibrary::IsValidClass(Data.Helm))
    {
        EquipmentClasses.Add(Data.Helm);
    }
    if (UKismetSystemLibrary::IsValidClass(Data.Boots))
    {
        EquipmentClasses.Add(Data.Boots);
    }
    return EquipmentClasses;
}