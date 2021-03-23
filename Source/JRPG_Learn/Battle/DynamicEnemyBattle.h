// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BattleBase.h"
#include "DynamicEnemyBattle.generated.h"

/**
 * 
 */
UCLASS()
class JRPG_LEARN_API ADynamicEnemyBattle : public ABattleBase
{
	GENERATED_BODY()
	
	public:

	virtual void StartBattle(TArray<FEnemyUnitSpawnDataList> EnemiesSpawnDatas, FOffLevelBattleData Data);

	protected:

	virtual TArray<FEnemyUnitSpawnDataList> GetEnemyList() override;
	virtual FOffLevelBattleData GetOffLevelBattleData() override;

	TArray<FEnemyUnitSpawnDataList> EnemyList;
	FOffLevelBattleData OffLevelBattleData;
};
