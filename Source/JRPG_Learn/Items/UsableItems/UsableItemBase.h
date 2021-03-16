// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../ItemBase.h"
#include "../../Data/Enums.h"
#include "UsableItemBase.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnItemUsedSignature)

class ABattleController;
class AJRPG_PlayerController;
class AUnitBase;
class APlayerUnitBase;

UCLASS()
class JRPG_LEARN_API AUsableItemBase : public AItemBase
{
	GENERATED_BODY()

	public:

	FOnItemUsedSignature OnItemUsed;

	virtual void Use(ABattleController*, AJRPG_PlayerController*, bool, AUnitBase*, TSubclassOf<APlayerUnitBase>);

	protected:

	virtual void OnUsedInBattle(AUnitBase*, TSubclassOf<APlayerUnitBase>);
	virtual void OnUsedOutsideBattle(TSubclassOf<APlayerUnitBase>);

	UPROPERTY(EditDefaultsOnly)
	EUnitFilter UnitFilter;

	TWeakObjectPtr<ABattleController> BattleController;
	TWeakObjectPtr<AJRPG_PlayerController> PlayerController;
};
