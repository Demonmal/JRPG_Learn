// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Engine/DataTable.h"
#include "DataTypes.generated.h"

class APlayerUnitBase;
class AEquipmentBase;
class AMiscItemBase;
class AUsableItemBase;
class AWeaponBase;
class AArmorBase;
class AShieldBase;
class AHelmBase;
class ABootsBase;
class USoundBase;

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

	UPROPERTY(EditDefaultsOnly)
	int Exp;
	UPROPERTY(EditDefaultsOnly)
	int CurrentHP;
	UPROPERTY(EditDefaultsOnly)
	int CurrentMP;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AWeaponBase> Weapon;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AArmorBase> Armor;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AShieldBase> Shield;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AHelmBase> Helm;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ABootsBase> Boots;
};

USTRUCT(BlueprintType)
struct FUnitStats : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	int MaxHP;
	UPROPERTY(EditDefaultsOnly)
	int MaxMP;
	UPROPERTY(EditDefaultsOnly)
	int MinAttack;
	UPROPERTY(EditDefaultsOnly)
	int MaxAttack;
	UPROPERTY(EditDefaultsOnly)
	int Defense;
	UPROPERTY(EditDefaultsOnly)
	int Speed;
	UPROPERTY(EditDefaultsOnly)
	int Hit;
	UPROPERTY(EditDefaultsOnly)
	int MinMagicalAttack;
	UPROPERTY(EditDefaultsOnly)
	int MaxMagicalAttack;
	UPROPERTY(EditDefaultsOnly)
	int MagicDefense;
	UPROPERTY(EditDefaultsOnly)
	float ActionTime;

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

	UPROPERTY(EditDefaultsOnly)
	USoundBase* NewQuestSound;
	UPROPERTY(EditDefaultsOnly)
	USoundBase* QuestUpdateSound;
	UPROPERTY(EditDefaultsOnly)
	USoundBase* QuestCompletedSound;
};

USTRUCT(BlueprintType)
struct FQuestRewards 
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<APlayerUnitBase> PlayerUnit;
	UPROPERTY(EditDefaultsOnly)
	TMap<TSubclassOf<AUsableItemBase>, int> UsableItems;
	UPROPERTY(EditDefaultsOnly)
	TMap<TSubclassOf<AMiscItemBase>, int> MiscItems;
	UPROPERTY(EditDefaultsOnly)
	TMap<TSubclassOf<AEquipmentBase>, int> Equipment;
	UPROPERTY(EditDefaultsOnly)
	int Gold;
};

USTRUCT(BlueprintType)
struct FEnemyUnitSpawnData
{
	GENERATED_BODY()

	float SpawnChance;
	int MinLevel;
	int MaxLevel;
};

USTRUCT(BlueprintType)
struct FThumbnail
{
	GENERATED_BODY()

	class UTexture2D* Thumbnail;
};

UCLASS()
class JRPG_LEARN_API UDataTypes : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
};
