// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DynamicEnemyBattle.h"
#include "InteractionBattle.generated.h"

/**
 * 
 */
UCLASS()
class JRPG_LEARN_API AInteractionBattle : public ADynamicEnemyBattle
{
	GENERATED_BODY()
	
	public:

	virtual void RemoveBattle() override;
};
