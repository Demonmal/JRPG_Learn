// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Engine/DataTable.h"
#include "DataTypes.generated.h"

class APlayerUnitBase;
class AEnemyUnitBase;
class AEquipmentBase;
class AMiscItemBase;
class AUsableItemBase;
class AWeaponBase;
class AArmorBase;
class AShieldBase;
class AHelmBase;
class ABootsBase;
class USoundBase;
class ABattleTransitionBase;

USTRUCT(BlueprintType)
struct FSound : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	USoundBase* Sound;
};

USTRUCT(BlueprintType)
struct FPlayerUnitData : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	int Exp;
	UPROPERTY(EditAnywhere)
	int CurrentHP;
	UPROPERTY(EditDefaultsOnly)
	int CurrentMP;
	UPROPERTY(EditAnywhere)
	TSubclassOf<AWeaponBase> Weapon;
	UPROPERTY(EditAnywhere)
	TSubclassOf<AArmorBase> Armor;
	UPROPERTY(EditAnywhere)
	TSubclassOf<AShieldBase> Shield;
	UPROPERTY(EditAnywhere)
	TSubclassOf<AHelmBase> Helm;
	UPROPERTY(EditAnywhere)
	TSubclassOf<ABootsBase> Boots;
};

USTRUCT(BlueprintType)
struct FUnitStats : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	int MaxHP = 0;
	UPROPERTY(EditAnywhere)
	int MaxMP = 0;
	UPROPERTY(EditAnywhere)
	int MinAttack = 0;
	UPROPERTY(EditAnywhere)
	int MaxAttack = 0;
	UPROPERTY(EditAnywhere)
	int Defense = 0;
	UPROPERTY(EditAnywhere)
	int Speed = 0;
	UPROPERTY(EditAnywhere)
	int Hit = 0;
	UPROPERTY(EditAnywhere)
	int MinMagicalAttack = 0;
	UPROPERTY(EditAnywhere)
	int MaxMagicalAttack = 0;
	UPROPERTY(EditAnywhere)
	int MagicDefense = 0;
	UPROPERTY(EditAnywhere)
	float ActionTime = 0.0f;

	FUnitStats operator+(const FUnitStats &Stats)
	{
		FUnitStats NewStats;
		NewStats.MaxHP = MaxHP + Stats.MaxHP;
		NewStats.MaxMP = MaxMP + Stats.MaxMP;
		NewStats.MinAttack = MinAttack + Stats.MinAttack;
		NewStats.MaxAttack = MaxAttack + Stats.MaxAttack;
		NewStats.Defense = Defense + Stats.Defense;
		NewStats.Speed = Speed + Stats.Speed;
		NewStats.Hit = Hit + Stats.Hit;
		NewStats.MinMagicalAttack = MinMagicalAttack + Stats.MinMagicalAttack;
		NewStats.MaxMagicalAttack = MaxMagicalAttack + Stats.MaxMagicalAttack;
		NewStats.MagicDefense = MagicDefense + Stats.MagicDefense;
		NewStats.ActionTime = ActionTime + Stats.ActionTime;
		return NewStats;
	}
};

USTRUCT(BlueprintType)
struct FQuestSounds 
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	USoundBase* NewQuestSound;
	UPROPERTY(EditAnywhere)
	USoundBase* QuestUpdateSound;
	UPROPERTY(EditAnywhere)
	USoundBase* QuestCompletedSound;
};

USTRUCT(BlueprintType)
struct FQuestRewards 
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	TSubclassOf<APlayerUnitBase> PlayerUnit;
	UPROPERTY(EditAnywhere)
	TMap<TSubclassOf<AUsableItemBase>, int> UsableItems;
	UPROPERTY(EditAnywhere)
	TMap<TSubclassOf<AMiscItemBase>, int> MiscItems;
	UPROPERTY(EditAnywhere)
	TMap<TSubclassOf<AEquipmentBase>, int> Equipment;
	UPROPERTY(EditAnywhere)
	int Gold;
};

USTRUCT(BlueprintType)
struct FEnemyUnitSpawnData : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	float SpawnChance;
	UPROPERTY(EditAnywhere)
	int MinLevel;
	UPROPERTY(EditAnywhere)
	int MaxLevel;
};

USTRUCT(BlueprintType)
struct FEnemyUnitSpawnDataList : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	TMap<TSubclassOf<AEnemyUnitBase>, FEnemyUnitSpawnData> EnemyList;
};

USTRUCT(BlueprintType)
struct FThumbnail
{
	GENERATED_BODY()

	class UTexture2D* Thumbnail;
};

USTRUCT(BlueprintType)
struct FOffLevelBattleData : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	FName BattleLevelName;
	UPROPERTY(EditAnywhere)
	TSubclassOf<ABattleTransitionBase> BattleLevelTransition;
};

UCLASS()
class JRPG_LEARN_API UDataTypes : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
};
