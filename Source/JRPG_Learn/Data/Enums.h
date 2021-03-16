// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Enums.generated.h"

UENUM(BlueprintType)
enum class EUnitClass : uint8
{
	All,
	Swordsman,
	Mage,
	Priest,
	Archer
};

UENUM(BlueprintType)
enum class EEquipmentType : uint8
{
	Weapon,
	Armor,
	Shield,
	Helm,
	Boots
};

UENUM(BlueprintType)
enum class EExploreCharacterMode : uint8
{
	FixedCharacter,
	PartyLeader,
	AllPartyMembers
};

UENUM(BlueprintType)
enum class EActionType : uint8
{
	None,
	Attack,
	Skill,
	Item,
	Flee,
	Interact
};

UENUM(BlueprintType)
enum class EGameState : uint8
{
	Explore,
	BattleTransition,
	Battle,
	Pause
};

UENUM(BlueprintType)
enum class EItemFilter : uint8
{
	All,
	UsableItems,
	MiscItems,
	Weapon,
	Armor,
	Shield,
	Helm,
	Boots
};

UENUM(BlueprintType)
enum class EQuestStatus : uint8
{
	Unavailable,
	New,
	Active,
	ReadyToDeliver,
	Completed
};

UENUM(BlueprintType)
enum class ETurnType : uint8
{
	ActionTime,
	Speed
};

UENUM(BlueprintType)
enum class EUnitFilter : uint8
{
	All,
	Alive,
	Dead
};

UENUM(BlueprintType)
enum class EDamageType : uint8
{
	Physical,
	Magical,
	Pure
};

UENUM(BlueprintType)
enum class ESkillType : uint8
{
	Offensive,
	Defensive
};

UCLASS()
class JRPG_LEARN_API UEnums : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
};
