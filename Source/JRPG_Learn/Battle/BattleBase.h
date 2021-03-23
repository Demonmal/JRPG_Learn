// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../Data/Enums.h"
#include "../Data/DataTypes.h"
#include "BattleBase.generated.h"

class AUnitBase;
class APlayerUnitBase;
class AEnemyUnitBase;
class AJRPG_PlayerController;
class ABattleTransitionBase;
class ABattleController;
class AUsableItemBase;
class AMiscItemBase;
class AEquipmentBase;
class AUnitSpawnLocation;
class UBillboardComponent;
class UStaticMeshComponent;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnBattleOverSignature, EBattleResult)
DECLARE_MULTICAST_DELEGATE(FOnBattleRemovedSignature)

UCLASS(Abstract)
class JRPG_LEARN_API ABattleBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABattleBase();

	FOnBattleOverSignature OnBattleOver;
	FOnBattleRemovedSignature OnBattleRemoved;

	virtual void InitBattle();
	virtual void InitUnits();
	virtual void SetEnemyUnitsDisableState(bool bIsDisabled);
	virtual void SetPlayerUnitsDisableState(bool bIsDisabled);
	virtual void AddActionTime(float ActionTime);
	virtual void Flee();
	virtual void RemoveBattle(){}
	virtual void OverlapStarted(){}

	bool TryGetPlayerUnitLevel(TSubclassOf<APlayerUnitBase> UnitClass, int& Level);

	FORCEINLINE bool IsDynamicCamera() const {return bIsDynamicCamera;}
	FORCEINLINE float GetRadius() const {return Radius;}
	FORCEINLINE float GetFleeChance() const {return FleeChance;}
	FORCEINLINE int GetTotalExpReward() const {return TotalExpReward;}
	FORCEINLINE int GetTotalGoldReward() const {return TotalGoldReward;}
	FORCEINLINE AActor *GetStaticCamera() const {return StaticCamera;}
	FORCEINLINE TArray<APlayerUnitBase*> GetPlayerUnits() const {return PlayerUnits;}
	FORCEINLINE TArray<AEnemyUnitBase*> GetEnemyUnits() const {return EnemyUnits;}
	FORCEINLINE AJRPG_PlayerController* GetPlayerController() const {return PlayerController.Get();}
	FORCEINLINE TSubclassOf<ABattleTransitionBase> GetOverrideBattleTransitionClass() const {return OverrideBattleTransition;}
	FORCEINLINE TMap<TSubclassOf<APlayerUnitBase>, FTransform> GetDeadPlayerUnits() const {return DeadPlayerUnits;}
	FORCEINLINE TMap<TSubclassOf<AUsableItemBase>, int> GetUsableItemDrops() const {return UsableItemDrops;}
	FORCEINLINE TMap<TSubclassOf<AMiscItemBase>, int> GetMiscItemDrops() const {return MiscItemDrops;}
	FORCEINLINE TMap<TSubclassOf<AEquipmentBase>, int> GetEquipmentDrops() const {return EquipmentDrops;}

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void InitPlayerSpawnLocations();
	virtual void SpawnPlayerUnits();
	virtual void SpawnEnemyUnits();
	virtual void DropUsableItems(AEnemyUnitBase * Unit);
	virtual void DropMiscItems(AEnemyUnitBase * Unit);
	virtual void DropEquipment(AEnemyUnitBase * Unit);
	virtual void InitPlayerUnit(AUnitBase *Unit);
	virtual TArray<FEnemyUnitSpawnDataList> GetEnemyList();
	virtual FOffLevelBattleData GetOffLevelBattleData();

	TMap<TSubclassOf<AEnemyUnitBase>, FEnemyUnitSpawnData> GetEnemySpawnData(int SpawnLocationIndex);
	void GetRandomEnemy(TMap<TSubclassOf<AEnemyUnitBase>, FEnemyUnitSpawnData>, TSubclassOf<AEnemyUnitBase>&, int&);
	void OnPlayerUnitDiedHandler(AUnitBase *Unit);
	void OnEnemyUnitDiedHandler(AUnitBase *Unit);
	
	UPROPERTY(VisibleDefaultsOnly)
	UBillboardComponent* BattleIcon;
	UPROPERTY(VisibleDefaultsOnly)
	UStaticMeshComponent* PlayerDetector;

	UPROPERTY(EditAnywhere)
	float Radius;
	UPROPERTY(EditAnywhere)
	bool bIsDynamicCamera;
	UPROPERTY(EditAnywhere)
	TSubclassOf<ABattleTransitionBase> OverrideBattleTransition;
	UPROPERTY(EditAnywhere)
	USoundBase *OverrideBattleTheme;
	UPROPERTY(EditAnywhere)
	float FleeChance;
	UPROPERTY(EditAnywhere)
	TMap<TSubclassOf<AUsableItemBase>, int> UsableItemDrops;
	UPROPERTY(EditAnywhere)
	TMap<TSubclassOf<AMiscItemBase>, int> MiscItemDrops;
	UPROPERTY(EditAnywhere)
	TMap<TSubclassOf<AEquipmentBase>, int> EquipmentDrops;
	UPROPERTY(EditAnywhere)
	TArray<AUnitSpawnLocation*> PlayerSpawnLocations;
	UPROPERTY(EditAnywhere)
	TArray<AUnitSpawnLocation*> EnemySpawnLocations;
	UPROPERTY(EditInstanceOnly)
	ABattleController *BattleController;
	UPROPERTY(EditInstanceOnly)
	AActor *StaticCamera;

	UPROPERTY()
	TArray<APlayerUnitBase*> PlayerUnits;
	UPROPERTY()
	TArray<AEnemyUnitBase*> EnemyUnits;
	UPROPERTY()
	TMap<TSubclassOf<APlayerUnitBase>, int> PlayerUnitLevels;
	UPROPERTY()
	TMap<TSubclassOf<APlayerUnitBase>, FTransform> DeadPlayerUnits;

	int TotalExpReward;
	int TotalGoldReward;

	TWeakObjectPtr<AJRPG_PlayerController> PlayerController;
};
