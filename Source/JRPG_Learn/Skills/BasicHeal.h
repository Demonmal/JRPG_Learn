// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BattleSkillBase.h"
#include "BasicHeal.generated.h"

/**
 * 
 */
UCLASS()
class JRPG_LEARN_API ABasicHeal : public ABattleSkillBase
{
	GENERATED_BODY()

	public:

	virtual void UseSkill() override;
	virtual void UseBattleSkillInExplore(TSubclassOf<APlayerUnitBase> TargetUnit) override;

	private:

	UPROPERTY(EditDefaultsOnly)
	int HealAmount = 30;

	const FName SoundTag = "Heal";
	const float DelayAfterUsage = 1.5f;
};
