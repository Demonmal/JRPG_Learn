// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UsableItemBase.h"
#include "RedPotion.generated.h"

/**
 * 
 */
UCLASS()
class JRPG_LEARN_API ARedPotion : public AUsableItemBase
{
	GENERATED_BODY()
	
	protected:

	virtual void OnUsedInBattle(AUnitBase*, TSubclassOf<APlayerUnitBase>) override;
	virtual void OnUsedOutsideBattle(TSubclassOf<APlayerUnitBase>) override;

	UPROPERTY(EditDefaultsOnly)
	float HealPercent = 0.15f;
};
