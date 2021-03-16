// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../ItemBase.h"
#include "../../Data/DataTypes.h"
#include "../../Data/Enums.h"
#include "EquipmentBase.generated.h"

class ASkillBase;

UCLASS()
class JRPG_LEARN_API AEquipmentBase : public AItemBase
{
	GENERATED_BODY()
public:
	UPROPERTY(EditDefaultsOnly)
	FUnitStats Stats;
	UPROPERTY(EditDefaultsOnly)
	TArray<TSubclassOf<class ABattleSkillBase>> BatlleSkills;
	UPROPERTY(EditDefaultsOnly)
	TArray<TSubclassOf<class AExploreSkillBase>> ExploreSkills;
	UPROPERTY(EditDefaultsOnly)
	EEquipmentType EquipmentType;
	UPROPERTY(EditDefaultsOnly)
	TArray<EUnitClass> RequiredUnitClasses;
	UPROPERTY(EditDefaultsOnly)
	class AEquipmentMeshBase* EquipmentMesh;
};
