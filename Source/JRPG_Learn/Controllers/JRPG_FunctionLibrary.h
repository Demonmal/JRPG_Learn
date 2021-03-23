// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "../Data/DataTypes.h"
#include "JRPG_FunctionLibrary.generated.h"

class ABattleSkillBase;
class AExploreSkillBase;
class AEquipmentBase;

UCLASS()
class JRPG_LEARN_API UJRPG_FunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	static FUnitStats GetUnitStats(int &Level, const int &MaxLevel, const FUnitStats &InitialStats, const FUnitStats &FirstLevelStats, const FUnitStats &LastLevelStats);
	UFUNCTION(BlueprintCallable)
	static int GetIntStat(int &Level, const int &MaxLevel, const int &InitialStat, const int &FirstLevelStat, const int &LastLevelStat);
	UFUNCTION(BlueprintCallable)
	static float GetActionTimeStat(int &Level, const int &MaxLevel, const float &FirstLevelStat, const float &LastLevelStat);
	UFUNCTION(BlueprintCallable)
	static void GetEquipmentSkills(const FPlayerUnitData& Data, TArray<TSubclassOf<ABattleSkillBase>> &EquipmentBattleSkills, TArray<TSubclassOf<AExploreSkillBase>> &EquipmentExploreSkills);
	UFUNCTION(BlueprintCallable)
	static void AppendSkillsFromEquipment(TSubclassOf<AEquipmentBase> EquipmentClass, TArray<TSubclassOf<ABattleSkillBase>> &EquipmentBattleSkills, TArray<TSubclassOf<AExploreSkillBase>> &EquipmentExploreSkills);
	UFUNCTION(BlueprintCallable)
	static FUnitStats GetEquipmentStats(const FPlayerUnitData& Data);
	UFUNCTION(BlueprintCallable)
	static FUnitStats GetEquipmentStat(TSubclassOf<AEquipmentBase> EquipmentClass);
	UFUNCTION(BlueprintCallable)
	static void MakeAbsoluteStats(FUnitStats &Stats);
	UFUNCTION(BlueprintCallable)
	static FUnitStats GetPlayerUnitStatsFromData(TSubclassOf<APlayerUnitBase> UnitClass, const FPlayerUnitData& Data, int& Level);
	UFUNCTION(BlueprintCallable)
	static int GetLevel(const int &Exp, const float &Exponent, const int &MaxLevel);
	UFUNCTION(BlueprintCallable)
	static TArray<UClass*> GetAllEquipmentClasses(const FPlayerUnitData& Data);
	UFUNCTION(BlueprintCallable)
	static void SetActorDisableState(AActor *Actor, bool bDisableState);
};
