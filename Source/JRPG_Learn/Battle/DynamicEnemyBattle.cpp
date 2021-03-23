// Fill out your copyright notice in the Description page of Project Settings.


#include "DynamicEnemyBattle.h"

void ADynamicEnemyBattle::StartBattle(TArray<FEnemyUnitSpawnDataList> EnemiesSpawnDatas, FOffLevelBattleData Data)
{
    EnemyList = EnemiesSpawnDatas;
    OffLevelBattleData = Data;
    InitBattle();
}

TArray<FEnemyUnitSpawnDataList> ADynamicEnemyBattle::GetEnemyList()
{
    return EnemyList;
}

FOffLevelBattleData ADynamicEnemyBattle::GetOffLevelBattleData()
{
    return OffLevelBattleData;
}